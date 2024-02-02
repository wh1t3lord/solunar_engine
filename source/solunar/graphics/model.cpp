#include "graphicspch.h"
#include "model.h"

#include "core/file/filesystem.h"
#include "core/file/datastream.h"
#include "core/file/contentmanager.h"

#include "graphics/shaderprogram.h"
#include "graphics/material.h"

#include "graphics/renderer.h"
#include "graphics/rendercontext.h"

#include "graphics/core/device.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include "core/file/filesystem.h"

#ifdef _MSC_VER
#	ifdef _DEBUG
#		pragma comment(lib, "assimp-vc141-mtd.lib")
#	else
#		pragma comment(lib, "assimp-vc141-mt.lib")
#	endif
#endif // _MSC_VER

namespace engine
{
	VertexFormat s_vfVertex;

	inline static glm::mat4 Assimp2Glm(const aiMatrix4x4& from)
	{
		return glm::mat4(
			(double)from.a1, (double)from.b1, (double)from.c1, (double)from.d1,
			(double)from.a2, (double)from.b2, (double)from.c2, (double)from.d2,
			(double)from.a3, (double)from.b3, (double)from.c3, (double)from.d3,
			(double)from.a4, (double)from.b4, (double)from.c4, (double)from.d4
		);
	}

	std::string getDirectoryPath(const std::string& filepath)
	{
		std::string directory = "";
		size_t last_slash_idx = filepath.rfind('/');
		if (std::string::npos == last_slash_idx)
			size_t last_slash_idx = filepath.rfind('\\');

		if (std::string::npos != last_slash_idx)
		{
			directory = filepath.substr(0, last_slash_idx);
		}
		return directory;
	}

	void osConvertPath(std::string& path)
	{
#ifdef WIN32
		for (int i = 0; i < path.size(); i++)
			if (path[i] == '/')
				path[i] = '\\';
#endif
	}

	void osConvertStandartPath(std::string& path)
	{
#ifdef WIN32
		for (int i = 0; i < path.size(); i++)
			if (path[i] == '\\')
				path[i] = '/';
#endif
	}

	SubMesh* ProccessSubMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
	{
		if (s_vfVertex.count() == 0)
		{
			s_vfVertex.addPosition(); //glm::vec3 m_position;
			s_vfVertex.addNormal();   //glm::vec3 m_normal;
			s_vfVertex.addTexcoord(); //glm::vec2 m_texcoord0;
			s_vfVertex.addNormal();   //glm::vec3 m_tangent;
			s_vfVertex.addNormal();   //glm::vec3 m_bitangent;
		}

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indecies;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.m_position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.m_normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			if (mesh->mTextureCoords[0])
			{
				vertex.m_texcoord0 = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

				if (mesh->mTangents)
					vertex.m_tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
				if (mesh->mBitangents)
					vertex.m_bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
			}
			else
			{
				vertex.m_texcoord0 = glm::vec2(0.0f, 0.0f);
			}

			//if (mesh->mTextureCoords[1])
			//	vertex.m_texcoord1 = glm::vec2(mesh->mTextureCoords[1][i].x, mesh->mTextureCoords[1][i].y);
			//else
			//	vertex.m_texcoord1 = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indecies.push_back(face.mIndices[j]);
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		char buffer[256];
		sprintf(buffer, "materials/%s.xml", material->GetName().C_Str());

		std::string bufferString = buffer;
		osConvertStandartPath(bufferString);

		if (!g_fileSystem->exist(bufferString.c_str()))
		{
			std::string directoryPath = getDirectoryPath(bufferString);
			osConvertPath(directoryPath);

	/*		if (!isDirectoryExist(directoryPath.c_str()))
				createDirectory(directoryPath.c_str());*/

			aiString diffusePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath);

			if (diffusePath.length == 0)
			{
				material->GetTexture(aiTextureType_DIFFUSE, 1, &diffusePath);
				if (diffusePath.length == 0)
				{
					material->GetTexture(aiTextureType_BASE_COLOR, 0, &diffusePath);
				}
			}


			aiString normalPath;
			material->GetTexture(aiTextureType_NORMALS, 0, &normalPath);

			Material::createMaterialFromImport(material->GetName().C_Str(), diffusePath.C_Str(), normalPath.C_Str());
		}

		aiMatrix4x4 nodePosition = node->mTransformation;
		glm::mat4 transform = glm::mat4(1.0f);
		transform = Assimp2Glm(nodePosition);

		return mem_new<SubMesh>(vertices, indecies, transform, material->GetName().C_Str());
	}

	void ProccessNode(std::vector<SubMesh*>& submeshes, aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			SubMesh* submesh = ProccessSubMesh(mesh, node, scene);
			submeshes.push_back(submesh);
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			ProccessNode(submeshes, node->mChildren[i], scene);
		}
	}

	ModelBase::ModelBase()
	{
	}

	ModelBase::~ModelBase()
	{
		destroy();
	}

	void ModelBase::registerObject()
	{
		TypeManager::getInstance()->registerObject<ModelBase>();
	}

	unsigned int kAssimpFlags = aiProcess_Triangulate | 
								aiProcess_PreTransformVertices | 
								aiProcess_GenSmoothNormals | 
								aiProcess_CalcTangentSpace;

	void ModelBase::load(const std::shared_ptr<DataStream>& dataStream)
	{
		dataStream->seek(Seek_End, 0);
		size_t filesize = dataStream->tell();
		dataStream->seek(Seek_Begin, 0);

		uint8_t* fileData = mem_array<uint8_t>(filesize);
		dataStream->read(fileData, filesize);

		Assimp::Importer importer;
		//const aiScene* scene = importer.ReadFile(m_filename.c_str(), aiProcess_Triangulate /*| aiProcess_TransformUVCoords | aiProcess_FlipUVs*/);
		const aiScene* scene = importer.ReadFileFromMemory(fileData, filesize, kAssimpFlags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Core::error("Failed to load model. %s", importer.GetErrorString());
		}

		Assert(scene);
		ProccessNode(m_subMeshes, scene->mRootNode, scene);

		mem_free_array(fileData);

		createHw();
	}

	void ModelBase::destroy()
	{
		for (int i = 0; i < m_subMeshes.size(); i++)
		{
			m_subMeshes[i]->destroy();
			mem_delete(m_subMeshes[i]);
		}

		m_subMeshes.clear();
	}

	void ModelBase::renderObjects()
	{
		for (int i = 0; i < m_subMeshes.size(); i++)
			m_subMeshes[i]->render();
	}

	void ModelBase::saveBinary(const std::string& filename)
	{
		//return;

		std::string oldFilename = filename;
		size_t extensionPos = oldFilename.find_last_of('.');
		if (extensionPos == std::string::npos)
			Core::error("ModelBase::saveBinary: invalid filename.");

		//std::string modelfilename = oldFilename.substr(0, extensionPos);
		std::string modelfilename = "data/";
		modelfilename += oldFilename.substr(0, extensionPos);
		modelfilename += ".model";

		if (g_fileSystem->exist(modelfilename.c_str()))
			return;

		//DataStreamPtr stream = g_fileSystem->openStream(modelfilename, false);
		//DataStreamPtr stream = std::make_shared<FileStream>(modelfilename, "wb");

		FileHandle file = g_fileSystem->create(modelfilename.c_str());

		ModelFileHeader header = { 0 };
		header.magic = MODELFILE_MAGIC;
		header.version = kModelFileVersion;
		header.submeshCount = m_subMeshes.size();
		g_fileSystem->write(file, &header, sizeof(file));

		for (SubMesh* pSubMesh : m_subMeshes)
		{
			ModelFileSubmeshData submeshData;
			memset(&submeshData, 0, sizeof(submeshData));
			//strcpy(submeshData.materialInfo, pSubMesh->getMaterial()->getName().c_str());
			strcpy(submeshData.materialInfo, "MATERIAL UNNAMED");
			submeshData.verticesCount = pSubMesh->getVerticesCount();
			submeshData.indicesCount = pSubMesh->getIndeciesCount();
			g_fileSystem->write(file, &submeshData, sizeof(submeshData));

			Vertex* pBufferVertices = (Vertex*)pSubMesh->getVertexBuffer()->map(BufferMapping::ReadOnly);

			// Avoid transform in vertex buffer
			// so we copy vertices to temp buffer and write them to file.
			Vertex* pVertices = (Vertex*)malloc(pSubMesh->getVerticesCount() * sizeof(Vertex));
			memcpy(pVertices, (const void*)pBufferVertices, pSubMesh->getVerticesCount() * sizeof(Vertex));

			// Unmap buffer
			pSubMesh->getVertexBuffer()->unmap();
			pBufferVertices = nullptr;

			// Transform vertices to OpenGL space.
			for (int i = 0; i < pSubMesh->getVerticesCount(); i++)
			{
				pVertices[i].m_position = pSubMesh->getTransform() * glm::vec4(pVertices[i].m_position, 0.0f);
			}

			g_fileSystem->write(file, pVertices, pSubMesh->getVerticesCount() * sizeof(Vertex));

			// free temp buffer
			free(pBufferVertices);

			unsigned int* pIndices = (unsigned int*)pSubMesh->getIndexBuffer()->map(BufferMapping::ReadOnly);
			g_fileSystem->write(file, pIndices, pSubMesh->getIndeciesCount() * sizeof(uint32_t));

			// Unmap buffer
			pSubMesh->getIndexBuffer()->unmap();
			pIndices = nullptr;
		}

		g_fileSystem->close(file);
	}

	void ModelBase::createHw()
	{
		for (int i = 0; i < m_subMeshes.size(); i++)
		{
			SubMesh* submesh = m_subMeshes[i];

			// Create vertex buffer

			BufferDesc vertexBufferDesc;
			memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
			vertexBufferDesc.m_bufferType = BufferType::VertexBuffer;
			vertexBufferDesc.m_bufferAccess = BufferAccess::Static;
			vertexBufferDesc.m_bufferMemorySize = submesh->m_vertices.size() * sizeof(Vertex);

			SubresourceDesc vertexBufferSubresourceDesc;
			memset(&vertexBufferSubresourceDesc, 0, sizeof(vertexBufferSubresourceDesc));
			vertexBufferSubresourceDesc.m_memory = submesh->m_vertices.data();

			submesh->m_vertexBuffer = g_renderDevice->createBuffer(vertexBufferDesc, vertexBufferSubresourceDesc);

			// Create index buffer

			BufferDesc indexBufferDesc;
			memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));
			indexBufferDesc.m_bufferType = BufferType::VertexBuffer;
			indexBufferDesc.m_bufferAccess = BufferAccess::Static;
			indexBufferDesc.m_bufferMemorySize = submesh->m_indecies.size() * sizeof(uint32_t);

			SubresourceDesc indexBufferSubresourceDesc;
			memset(&indexBufferSubresourceDesc, 0, sizeof(indexBufferSubresourceDesc));
			indexBufferSubresourceDesc.m_memory = submesh->m_indecies.data();

			submesh->m_indexBuffer = g_renderDevice->createBuffer(indexBufferDesc, indexBufferSubresourceDesc);

			// Load material

			// submesh->m_material = ContentManager::getInstance()->loadMaterial(submesh->m_materialName);
			//submesh->m_material = dynamicCastPtr<Material>(g_contentManager->load(submesh->m_materialName, Material::getStaticTypeInfo()));
			submesh->m_material = g_contentManager->loadObject<Material>(submesh->m_materialName);
			if (submesh->m_material.lock() == nullptr)
			{
				//Core::msg("ERROR: ModelBase: Missing material \"%s\"", submesh->m_materialName.c_str());
				submesh->m_material = getDefaultMaterial();
			}
		}
	}

	void ModelBase::releaseHw()
	{
		destroy();
	}

	SubMesh::SubMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname) :
		m_vertexBuffer(0),
		m_indexBuffer(0)
	{
		load(vertices, indecies, position, materialname);
	}

	SubMesh::SubMesh() :
		m_vertexBuffer(0),
		m_indexBuffer(0)
	{

	}

	void SubMesh::load(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname)
	{
		//m_materialName = materialname;
		m_materialName += "materials/";
		m_materialName += materialname;
		m_materialName += ".xml";

		m_transform = position;

		m_vertices = vertices;
		m_indecies = indecies;
		m_verticesCount = vertices.size();
		m_indeciesCount = indecies.size();
	}

	SubMesh::~SubMesh()
	{
		destroy();
	}

	void SubMesh::destroy()
	{
		m_material.reset();

		if (m_vertexBuffer)
		{
			mem_delete(m_vertexBuffer);
			m_vertexBuffer = nullptr;
		}
	}

	void SubMesh::render()
	{
		//create saved render ctx as previous model.
		//RenderContext savedCtx = RenderContext::getContext();

		//// create local copy of render context
		//RenderContext localCtx = RenderContext::getContext();

		//// and overwrite model matrix
		//localCtx.model = savedCtx.model * m_transform;

		//// set our local render ctx
		//RenderContext::setContext(localCtx);

		//GraphicsDevice::getInstance()->setVertexBuffer(m_vertexBuffer);
		//g_renderDevice->setVertexFormat(&s_vfVertex);
		//
		//m_material->bind();

		//GraphicsDevice::getInstance()->setIndexBuffer(m_indexBuffer);

		////GraphicsDevice::getInstance()->drawArray(PrimitiveMode::Triangles, 0, m_verticesCount);
		//GraphicsDevice::getInstance()->drawElements(PrimitiveMode::Triangles, m_indeciesCount);

		//// return what have been
		//RenderContext::setContext(savedCtx);

		//// reset material bindings
		//m_material->resetAllStates();
	}

	std::shared_ptr<Material> SubMesh::lockMaterial()
	{
		return m_material.lock();
	}

	void ModelSubmesh::registerObject()
	{
		TypeManager::getInstance()->registerObject<ModelSubmesh>();
	}

	void ModelSubmesh::load(DataStreamPtr stream)
	{
		// read submesh data
		ModelFileSubmeshData submeshData = { 0 };
		stream->read(&submeshData);

		m_materialName = submeshData.materialInfo;
		m_material = dynamicCastPtr<Material>(g_contentManager->load(m_materialName, Material::getStaticTypeInfo()));

		Assert2(submeshData.verticesCount != 0, "Mesh has zero vertices, internal error...");
		Assert2(submeshData.indicesCount != 0, "Mesh has zero indices, internal error...");

		for (int i = 0; i < submeshData.verticesCount; i++)
		{
			Vertex vertex;
			stream->read(&vertex);

			m_vertices.push_back(vertex);
		}

		m_verticesCount = m_vertices.size();

		for (int i = 0; i < submeshData.indicesCount; i++)
		{
			unsigned int index;
			stream->read(&index);

			m_indices.push_back(index);
		}

		m_indicesCount = m_indices.size();
	}

	void ModelSubmesh::save(DataStreamPtr stream)
	{
	}

	void ModelSubmesh::createHw()
	{
		/////////////////////////
		// Vertex buffer creation
		/////////////////////////

		BufferDesc verticesBufferDesc;
		memset(&verticesBufferDesc, 0, sizeof(verticesBufferDesc));
		verticesBufferDesc.m_bufferType = BufferType::VertexBuffer;
		verticesBufferDesc.m_bufferAccess = BufferAccess::Static;
		verticesBufferDesc.m_bufferMemorySize = m_vertices.size() * sizeof(Vertex);

		SubresourceDesc verticesSubresourceDesc;
		memset(&verticesSubresourceDesc, 0, sizeof(verticesSubresourceDesc));
		verticesSubresourceDesc.m_memory = m_vertices.data();

		m_verticesBuffer = g_renderDevice->createBuffer(verticesBufferDesc, verticesSubresourceDesc);

		////////////////////////
		// Index buffer creation
		////////////////////////

		BufferDesc indicesBufferDesc;
		memset(&indicesBufferDesc, 0, sizeof(indicesBufferDesc));
		indicesBufferDesc.m_bufferType = BufferType::IndexBuffer;
		indicesBufferDesc.m_bufferAccess = BufferAccess::Static;
		indicesBufferDesc.m_bufferMemorySize = m_indices.size() * sizeof(unsigned int);

		SubresourceDesc indicesSubresourceDesc;
		memset(&indicesSubresourceDesc, 0, sizeof(indicesSubresourceDesc));
		indicesSubresourceDesc.m_memory = m_indices.data();

		m_indicesBuffer = g_renderDevice->createBuffer(indicesBufferDesc, indicesSubresourceDesc);

		// clear temp data
		m_indices.clear();
		m_vertices.clear();
	}

	void ModelSubmesh::releaseHw()
	{
	}

	void Model::registerObject()
	{
		TypeManager::getInstance()->registerObject<Model>();
	}

	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	template <typename T>
	void templatedObjectDeleter(T* t)
	{
		if (t)
		{
			mem_delete(t);
		}
	}

	void submeshDeleter(ModelSubmesh* submesh)
	{
		if (submesh)
		{
			mem_delete(submesh);
		}
	}

	void Model::load(const std::shared_ptr<DataStream>& stream)
	{
		// read header
		ModelFileHeader header;
		stream->read(&header);

		if (header.magic != MODELFILE_MAGIC)
			Core::error("Model file has unknowed magic!");

		if (header.version > kModelFileVersion)
			Core::msg("[graphics]: model has older version format");
		else if (header.version < kModelFileVersion)
			Core::error("Model has newer version than engine support. (model %i, engine %i)", header.version, kModelFileVersion);

		if (header.submeshCount == 0)
			Core::error("Model has zero sub meshes, cannot load");

		for (int i = 0; i < header.submeshCount; i++)
		{
			// Create submesh
			ModelSubmesh* submeshPtr = (ModelSubmesh*)TypeManager::getInstance()->createObjectByTypeInfo(ModelSubmesh::getStaticTypeInfo());
		
			std::shared_ptr<ModelSubmesh> submesh = std::shared_ptr<ModelSubmesh>(submeshPtr, templatedObjectDeleter<ModelSubmesh>);

			// load from current stream
			submesh->load(stream);
		}
	}

	void Model::save(const std::shared_ptr<DataStream>& stream)
	{
		ModelFileHeader header = { 0 };
		header.magic = MODELFILE_MAGIC;
		header.version = kModelFileVersion;
		header.submeshCount = m_submeshes.size();
		stream->write(&header);

		for (auto pSubMesh : m_submeshes)
		{
			ModelFileSubmeshData submeshData;
			memset(&submeshData, 0, sizeof(submeshData));
			//strcpy(submeshData.materialInfo, pSubMesh->getMaterial()->getName().c_str());
			strcpy(submeshData.materialInfo, "MATERIAL UNNAMED");
			submeshData.verticesCount = pSubMesh->getVerticesCount();
			submeshData.indicesCount = pSubMesh->getIndicesCount();
			stream->write(&submeshData);

			Vertex* pVertices = (Vertex*)pSubMesh->getVerticesBuffer()->map(BufferMapping::ReadOnly);
			stream->write(pVertices, pSubMesh->getVerticesCount() * sizeof(Vertex));

			// Unmap buffer
			pSubMesh->getVerticesBuffer()->unmap();
			pVertices = nullptr;

			unsigned int* pIndices = (unsigned int*)pSubMesh->getIndicesBuffer()->map(BufferMapping::ReadOnly);
			stream->write(pIndices, pSubMesh->getIndicesCount() * sizeof(uint32_t));

			// Unmap buffer
			pSubMesh->getIndicesBuffer()->unmap();
			pIndices = nullptr;
		}

		//stream->flush();
	}

	void Model::createHw()
	{
	}

	void Model::releaseHw()
	{
	}

}
