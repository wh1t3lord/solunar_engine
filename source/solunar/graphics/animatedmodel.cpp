#include "graphicspch.h"

#include "core/file/datastream.h"

// Graphics core
#include "graphics/core/device.h"
#include "graphics/core/texture.h"
#include "graphics/core/buffer.h"

#include "graphics/animatedmodel.h"

namespace engine
{

//const unsigned int kAssimpFlags = 
//	aiProcessPreset_TargetRealtime_Quality |                     // some optimizations and safety checks
//	aiProcess_OptimizeMeshes |                                   // minimize number of meshes
//	//aiProcess_PreTransformVertices |                             // apply node matrices
//	//	aiProcess_Triangulate |
//	aiProcess_SplitLargeMeshes |
//	aiProcess_TransformUVCoords /*|*/ // apply UV transformations
//	/*aiProcess_FlipUVs*/;
//
//inline static glm::mat4 Assimp2Glm(const aiMatrix4x4& from)
//{
//	return glm::mat4(
//		(double)from.a1, (double)from.b1, (double)from.c1, (double)from.d1,
//		(double)from.a2, (double)from.b2, (double)from.c2, (double)from.d2,
//		(double)from.a3, (double)from.b3, (double)from.c3, (double)from.d3,
//		(double)from.a4, (double)from.b4, (double)from.c4, (double)from.d4
//	);
//}
//
//void SetVertexBoneData(AnimatedVertex& vertex, int boneID, float weight)
//{
//	for (int i = 0; i < MAX_BONE_WEIGHT; ++i)
//	{
//		if (vertex.m_boneIDs[i] < 0)
//		{
//			vertex.m_weights[i] = weight;
//			vertex.m_boneIDs[i] = boneID;
//			break;
//		}
//	}
//}
//
//void ExtractBoneWeightForVertices(AnimatedModel* model, std::vector<AnimatedVertex>& vertices, aiMesh* mesh, const aiScene* scene)
//{
//	auto& boneInfoMap = model->getBoneMapInfo(); //m_BoneInfoMap;
//	int& boneCount = model->getBoneCount(); // m_BoneCounter;
//
//	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
//	{
//		int boneID = -1;
//		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
//		if (boneInfoMap.find(boneName) == boneInfoMap.end())
//		{
//			AnimatedModelBoneInfo newBoneInfo;
//			newBoneInfo.m_id = boneCount;
//			newBoneInfo.m_offset = Assimp2Glm(mesh->mBones[boneIndex]->mOffsetMatrix);
//			boneInfoMap[boneName] = newBoneInfo;
//			boneID = boneCount;
//			boneCount++;
//		}
//		else
//		{
//			boneID = boneInfoMap[boneName].m_id;
//		}
//
//		Assert(boneID != -1);
//		auto weights = mesh->mBones[boneIndex]->mWeights;
//		int numWeights = mesh->mBones[boneIndex]->mNumWeights;
//
//		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
//		{
//			int vertexId = weights[weightIndex].mVertexId;
//			float weight = weights[weightIndex].mWeight;
//			Assert(vertexId <= vertices.size());
//			SetVertexBoneData(vertices[vertexId], boneID, weight);
//		}
//	}
//}
//
//AnimatedSubMesh* ProccessAnimatedSubMesh(AnimatedModel* model, aiMesh* mesh, aiNode* node, const aiScene* scene)
//{
//	std::vector<AnimatedVertex> vertices;
//	std::vector<uint32_t> indecies;
//
//	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
//	{
//		AnimatedVertex vertex;
//		vertex.m_position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
//		vertex.m_normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
//
//		if (mesh->mTextureCoords[0])
//			vertex.m_texcoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
//		else
//			vertex.m_texcoord = glm::vec2(0.0f, 0.0f);
//
//		if (mesh->mTangents)
//			vertex.m_tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
//		else
//			vertex.m_tangent = glm::vec3(0.0f);
//
//		if (mesh->mBitangents)
//			vertex.m_bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
//		else
//			vertex.m_bitangent = glm::vec3(0.0f);
//
//		for (int i = 0; i < MAX_BONE_WEIGHT; i++)
//		{
//			vertex.m_boneIDs[i] = -1;
//			vertex.m_weights[i] = 0.0f;
//		}
//
//		vertices.push_back(vertex);
//	}
//
//
//	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
//	{
//		aiFace face = mesh->mFaces[i];
//
//		for (uint32_t j = 0; j < face.mNumIndices; j++)
//			indecies.push_back(face.mIndices[j]);
//	}
//
//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//
//	char buffer[256];
//	sprintf(buffer, "data/materials/%s.xml", material->GetName().C_Str());
//
//	std::string bufferString = buffer;
//	osConvertStandartPath(bufferString);
//
//	if (!g_fileSystem->exist(bufferString.c_str()))
//	{
//		std::string directoryPath = getDirectoryPath(bufferString);
//		osConvertPath(directoryPath);
//
//		/*		if (!isDirectoryExist(directoryPath.c_str()))
//					createDirectory(directoryPath.c_str());*/
//
//		aiString diffusePath;
//		material->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath);
//
//		if (diffusePath.length == 0)
//		{
//			material->GetTexture(aiTextureType_DIFFUSE, 1, &diffusePath);
//			if (diffusePath.length == 0)
//			{
//				material->GetTexture(aiTextureType_BASE_COLOR, 0, &diffusePath);
//			}
//		}
//
//
//		aiString normalPath;
//		material->GetTexture(aiTextureType_NORMALS, 0, &normalPath);
//
//		Material::createMaterialFromImport(material->GetName().C_Str(), diffusePath.C_Str(), normalPath.C_Str());
//	}
//
//	aiMatrix4x4 nodePosition = node->mTransformation;
//	glm::mat4 transform = glm::mat4(1.0f);
//	transform = Assimp2Glm(nodePosition);
//
//	return nullptr;
//
//	//return mem_new<SubMesh>(vertices, indecies, transform, material->GetName().C_Str());
//}
//
//void ProccessAnimatedNode(AnimatedModel* model, std::vector<AnimatedSubMesh*>& submeshes, aiNode* node, const aiScene* scene)
//{
//	for (uint32_t i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		AnimatedSubMesh* submesh = ProccessAnimatedSubMesh(model, mesh, node, scene);
//		submeshes.push_back(submesh);
//	}
//
//	for (uint32_t i = 0; i < node->mNumChildren; i++)
//		ProccessAnimatedNode(model, submeshes, node->mChildren[i], scene);
//}

// Object registering
void AnimatedModel::registerObject()
{
	TypeManager::getInstance()->registerObject<AnimatedModel>();
}

AnimatedModel::AnimatedModel()
{
	m_boneCount = 0;
}

AnimatedModel::~AnimatedModel()
{
}

void AnimatedModel::load(const std::shared_ptr<DataStream>& stream)
{
	//stream->seek(Seek_End, 0);
	//long fileLenght = stream->tell();
	//stream->seek(Seek_Begin, 0);

	//uint8_t* fileData = mem_array<uint8_t>(fileLenght);
	//stream->read(fileData, fileLenght);

	//Assimp::Importer importer;
	////const aiScene* scene = importer.ReadFile(m_filename.c_str(), aiProcess_Triangulate /*| aiProcess_TransformUVCoords | aiProcess_FlipUVs*/);
	//const aiScene* scene = importer.ReadFileFromMemory(fileData, fileLenght, kAssimpFlags);

	//if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	//{
	//	Core::error("Failed to load model. %s", importer.GetErrorString());
	//}

	//Assert(scene);

	//ProccessAnimatedNode(this, m_subMeshes, scene->mRootNode, scene);

	//for (int i = 0; i < scene->mNumAnimations; i++)
	//{
	//	aiAnimation* ani = scene->mAnimations[i];
	//	if (ani)
	//	{
	//		Core::msg("Ani: name %s, duration: %.1f", 
	//			ani->mName.C_Str(),
	//			(float)ani->mDuration);
	//	}
	//}

	//mem_free_array(fileData);

	//createHw();
}

void AnimatedModel::createHw()
{
}

void AnimatedModel::releaseHw()
{
}

///////////////////////////////////////////////////////////
// Animated Model Texture Renderer

AnimatedModelRenderer AnimatedModelRenderer::ms_instance;

AnimatedModelRenderer::AnimatedModelRenderer()
{
	m_jointTexture = nullptr;
	m_jointSampler = nullptr;
}

AnimatedModelRenderer::~AnimatedModelRenderer()
{
}

void AnimatedModelRenderer::init()
{
	// create a dynamic joint-palette texture and sampler

	TextureDesc textureDesc;
	memset(&textureDesc, 0, sizeof(textureDesc));
	textureDesc.m_textureType = TextureType::Texture2D;
	textureDesc.m_width = MAX_JOINTS * 3;
	textureDesc.m_height = MAX_INSTANCES;
	textureDesc.m_mipmapLevel = 1;
	textureDesc.m_format = ImageFormat::RGBA32F;
	textureDesc.m_isStreamTexture = true;

	SubresourceDesc subresourceDesc;
	memset(&subresourceDesc, 0, sizeof(subresourceDesc));
	subresourceDesc.m_memoryPitch = textureDesc.m_width * 4;

	m_jointTexture = g_renderDevice->createTexture2D(textureDesc, subresourceDesc);

	SamplerDesc samplerDesc;
	memset(&samplerDesc, 0, sizeof(samplerDesc));
	samplerDesc.m_minFilter = TextureFilter::Nearest;
	samplerDesc.m_magFilter = TextureFilter::Nearest;
	samplerDesc.m_wrapS = TextureWrap::ClampToEdge;
	samplerDesc.m_wrapT = TextureWrap::ClampToEdge;
	samplerDesc.m_anisotropyLevel = 1.0f;

	m_jointSampler = g_renderDevice->createSamplerState(samplerDesc);
}

void AnimatedModelRenderer::shutdown()
{
	if (m_jointSampler)
	{
		mem_delete(m_jointSampler);
		m_jointSampler = nullptr;
	}

	if (m_jointTexture)
	{
		mem_delete(m_jointTexture);
		m_jointTexture = nullptr;
	}
}

void AnimatedModelRenderer::render(AnimatedModel* model)
{
	// compute skinning matrices and write to joint texture upload buffer

	glm::mat4* skinningMatrices = mem_array<glm::mat4>(256);
	for (int i = 0; i < 256; i++) {

	}
}

}