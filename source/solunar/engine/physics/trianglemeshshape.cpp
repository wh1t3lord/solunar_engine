#include "enginepch.h"
#include "engine/physics/trianglemeshshape.h"

#include "core/file/filesystem.h"
#include "core/file/contentmanager.h"

#include "graphics/model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace solunar
{

// not good but normal for our needs
uint64_t generate_fast_hash(const char* str)
{
	uint64_t ullHash = 0ll;
	for (; *str; str++)
		ullHash += uint64_t(*str);
	return (ullHash);
}

uint64_t get_cm_magic()
{
	// calculate it only once
	static uint64_t ullMagic = generate_fast_hash(CM_MAGIC);
	return (ullMagic);
}

static TriangleMeshServer s_triangleMeshServer;

TriangleMeshServer::TriangleMeshServer()
{
}

TriangleMeshServer::~TriangleMeshServer()
{
}

void TriangleMeshServer::SaveCollision(const char* modelfilename)
{
	char buffer[260];
	snprintf(buffer, sizeof(buffer), "data/models/%s.cm", modelfilename);

	FileHandle file = g_fileSystem->Create(buffer);

	/////////////////////////////////////////////////////
	// Write header
	uint64_t ullMagic = get_cm_magic();
	g_fileSystem->Write(file, &ullMagic, sizeof(ullMagic));

	float fVersion = CM_VERSION;
	g_fileSystem->Write(file, &fVersion, sizeof(fVersion));

	g_fileSystem->Close(file);
}

struct CollisionMeshAssImp
{
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	glm::mat4 transform;
};

static glm::mat4 Assimp2Glm(const aiMatrix4x4& from)
{
	return glm::mat4(
		(double)from.a1, (double)from.b1, (double)from.c1, (double)from.d1,
		(double)from.a2, (double)from.b2, (double)from.c2, (double)from.d2,
		(double)from.a3, (double)from.b3, (double)from.c3, (double)from.d3,
		(double)from.a4, (double)from.b4, (double)from.c4, (double)from.d4
	);
}

static CollisionMeshAssImp ProccessSubMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
{
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertices.push_back(pos);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMatrix4x4 nodePosition = node->mTransformation;
	glm::mat4 transform = glm::mat4(1.0f);
	transform = Assimp2Glm(nodePosition);

	CollisionMeshAssImp mesh2;
	mesh2.transform = transform;
	mesh2.vertices = vertices;
	mesh2.indices = indices;

	return mesh2;
}

static void ProccessNode(std::vector<CollisionMeshAssImp>& submeshes, aiNode* node, const aiScene* scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		CollisionMeshAssImp submesh = ProccessSubMesh(mesh, node, scene);
		submeshes.push_back(submesh);
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ProccessNode(submeshes, node->mChildren[i], scene);
	}
}

inline btVector3 getBulletVectorFromGlm(const glm::vec3& vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}

/////////////////////////////////////////////////////////////////////

ATTRIBUTE_ALIGNED16(class)
HackCompoundShape : public btCompoundShape
{
public:
	HackCompoundShape() {}
	~HackCompoundShape()
	{
		releaseChilds();
	}

	void releaseChilds()
	{
		std::vector<btCollisionShape*> shapes;
		for (int i = 0; i < m_children.size(); i++)
			shapes.push_back(m_children[i].m_childShape);
		for (int i = 0; i < shapes.size(); i++)
			removeChildShape(shapes[i]);
	}
};

/////////////////////////////////////////////////////////////////////

IMPLEMENT_OBJECT(TriangleMeshShapeComponent, ShapeComponent);

TriangleMeshShapeComponent::TriangleMeshShapeComponent()
{
}

TriangleMeshShapeComponent::~TriangleMeshShapeComponent()
{
	// delete shapes
	for (int i = 0; i < m_trishapes.size(); i++)
		mem_delete(m_trishapes[i]);

	// delete meshes
	for (int i = 0; i < m_trimeshes.size(); i++)
		mem_delete(m_trimeshes[i]);

	m_trishapes.clear();
	m_trimeshes.clear();
}

void TriangleMeshShapeComponent::RegisterObject()
{
	g_typeManager->RegisterObject<TriangleMeshShapeComponent>();
}

void TriangleMeshShapeComponent::LoadXML(tinyxml2::XMLElement& element)
{
	ShapeComponent::LoadXML(element);

	tinyxml2::XMLElement* modelElement = element.FirstChildElement("Model");
	if (modelElement)
	{
		const tinyxml2::XMLAttribute* filenameAttribute = modelElement->FindAttribute("filename");
		if (filenameAttribute && strlen(filenameAttribute->Value()) > 0)
		{
			m_filename = filenameAttribute->Value();
		}
		else
		{
			Core::Msg("WARNING: TriangleMeshShapeComponent at entity(0x%p) has empty filename attribute in Model element!", GetEntity());
		}
	}
}

void TriangleMeshShapeComponent::SaveXML(tinyxml2::XMLElement& element)
{
	ShapeComponent::SaveXML(element);
}

void TriangleMeshShapeComponent::CreateShapeInternal()
{
	if (m_filename.empty())
	{
		Core::Msg("TriangleMeshShapeComponent::CreateShapeInternal: filename is empty.");
		return;
	}

	if (m_filename.find(".model") != std::string::npos)
	{
		CreateShapeInternal_Model();
	}
	else
	{
		CreateShapeInternal_AssImp();
	}
}

void TriangleMeshShapeComponent::CreateShapeInternal_AssImp()
{
	DataStreamPtr stream = g_contentManager->OpenStream(m_filename);
	if (!stream)
	{
		Core::Msg("TriangleMeshShapeComponent::CreateShapeInternal: failed to open file %s", m_filename.c_str());
		return;
	}

	stream->Seek(Seek_End, 0);
	size_t length = stream->Tell();
	stream->Seek(Seek_Begin, 0);

	char* filedata = new char[length];
	stream->Read(filedata, length);

	stream = nullptr;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(filedata, length,
		aiProcess_OptimizeMeshes |
		aiProcess_JoinIdenticalVertices |
		aiProcess_PreTransformVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		Core::Error("TriangleMeshShapeComponent::CreateShapeInternal: failed to load scene '%s'.\n%s", m_filename.c_str(), importer.GetErrorString());
	}

	if (scene && !scene->HasMeshes()) {
		Core::Error("TriangleMeshShapeComponent::CreateShapeInternal: model '%s' doesnt have any mesh. Please check export parameters!", m_filename.c_str());
	}

	std::vector<CollisionMeshAssImp> out;
	ProccessNode(out, scene->mRootNode, scene);

	delete[] filedata;

	size_t verticesCount = 0;
	size_t indicesCount = 0;
	size_t trianglesCount = 0;

	m_shape = mem_new<HackCompoundShape>();

	for (int i = 0; i < out.size(); i++)
	{
		const CollisionMeshAssImp& collisionMeshAssImp = out[i];

		btTriangleMesh* collisionMesh = mem_new<btTriangleMesh>();
		m_trimeshes.push_back(collisionMesh);

		for (unsigned int n = 0; n < collisionMeshAssImp.indices.size(); n += 3) {
			collisionMesh->addTriangle(
				getBulletVectorFromGlm(collisionMeshAssImp.vertices[collisionMeshAssImp.indices[n]]),
				getBulletVectorFromGlm(collisionMeshAssImp.vertices[collisionMeshAssImp.indices[n + 1]]),
				getBulletVectorFromGlm(collisionMeshAssImp.vertices[collisionMeshAssImp.indices[n + 2]]),
				true);

			trianglesCount++;
		}

		btBvhTriangleMeshShape* collisionShape = mem_new<btBvhTriangleMeshShape>(collisionMesh, true);
		m_trishapes.push_back(collisionShape);

		btTransform trans;
		trans.setIdentity();

		((btCompoundShape*)m_shape)->addChildShape(trans, collisionShape);
	}

	Core::Msg("TriangleMeshShapeComponent(%s): %i triangles %i bytes", m_filename.c_str(), trianglesCount, trianglesCount * sizeof(glm::vec3));
}

void TriangleMeshShapeComponent::CreateShapeInternal_Model()
{
	DataStreamPtr stream = g_contentManager->OpenStream(m_filename);
	if (!stream)
	{
		Core::Msg("TriangleMeshShapeComponent::CreateShapeInternal: failed to open file %s", m_filename.c_str());
		return;
	}

	// read header
	ModelFileHeader header;
	stream->Read(&header);

	if (header.magic != MODELFILE_MAGIC)
		Core::Error("Model file has unknowed magic!");

	if (header.version > kModelFileVersion)
		Core::Msg("[graphics]: model has older version format");
	else if (header.version < kModelFileVersion)
		Core::Msg("Model has newer version than engine support. (model %i, engine %i)", header.version, kModelFileVersion);

	if (header.submeshCount == 0)
		Core::Msg("Model has zero sub meshes, cannot load");

	size_t verticesUsageCount = 0;
	size_t indicesUsageCount = 0;
	size_t trianglesUsageCount = 0;

	if (header.submeshCount > 0)
		m_shape = mem_new<HackCompoundShape>();

	for (uint32_t i = 0; i < header.submeshCount; i++)
	{
		// read submesh data
		ModelFileSubmeshData submeshData = { 0 };
		stream->Read(&submeshData);

		std::vector<Vertex> vertices;
		uint32_t verticesCount = submeshData.verticesCount;
		vertices.resize(verticesCount);
		stream->Read(vertices.data(), verticesCount * sizeof(Vertex));

		verticesUsageCount += (size_t)vertices.size();

		std::vector<unsigned int> indices;

		uint32_t indicesCount = submeshData.indicesCount;
		indices.resize(indicesCount);
		stream->Read(indices.data(), indicesCount * sizeof(unsigned int));

		indicesUsageCount += (size_t)indices.size();
	
		btTriangleMesh* collisionMesh = mem_new<btTriangleMesh>();
		m_trimeshes.push_back(collisionMesh);
		
		for (unsigned int n = 0; n < indices.size(); n += 3) {
			collisionMesh->addTriangle(
				getBulletVectorFromGlm(vertices[indices[n]].m_position),
				getBulletVectorFromGlm(vertices[indices[n + 1]].m_position),
				getBulletVectorFromGlm(vertices[indices[n + 2]].m_position),
				true);

			trianglesUsageCount++;
		}

		btBvhTriangleMeshShape* collisionShape = mem_new<btBvhTriangleMeshShape>(collisionMesh, true);
		m_trishapes.push_back(collisionShape);

		btTransform trans;
		trans.setIdentity();

		((btCompoundShape*)m_shape)->addChildShape(trans, collisionShape);
	}

	Core::Msg("TriangleMeshShapeComponent(%s): %i triangles %i bytes", m_filename.c_str(), trianglesUsageCount, trianglesUsageCount * sizeof(glm::vec3));
}

}