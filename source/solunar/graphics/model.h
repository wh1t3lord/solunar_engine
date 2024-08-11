#ifndef MODEL_H
#define MODEL_H

#include "core/math/boundingbox.h"
#include "graphics/graphicsobject.h"

namespace solunar
{
	struct Vertex
	{
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_texcoord0;
		//glm::vec2 m_texcoord1;

		glm::vec3 m_tangent;
		glm::vec3 m_bitangent;
	};

	class ModelBase;
	class Material;
	struct IBufferBase;

	//IBufferBase* createVBFromModel(VertexFormat* meshVF, int countOfVertexDecls, const std::string& filename, int* verticesCount);

	class SubMesh
	{
		friend class ModelBase;
	public:
		SubMesh();
		SubMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);
		void load(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);

		~SubMesh();

		void Destroy();

		void render();

		IBufferBase* getVertexBuffer() { return m_vertexBuffer; }
		IBufferBase* getIndexBuffer() { return m_indexBuffer; }

		const uint32_t getVerticesCount() const { return m_verticesCount; }
		const uint32_t getIndeciesCount() const { return m_indeciesCount; }

		const glm::mat4 getTransform() const { return m_transform; }

		std::weak_ptr<Material> getMaterial() { return m_material; }
		std::shared_ptr<Material> lockMaterial();

	private:
		std::weak_ptr<Material> m_material;
		std::string m_materialName;

		IBufferBase* m_vertexBuffer;
		IBufferBase* m_indexBuffer;

		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indecies;

		glm::mat4 m_transform;

		uint32_t m_verticesCount;
		uint32_t m_indeciesCount;
	};

	class ModelBase : public GraphicsObject
	{
		IMPLEMENT_OBJECT(ModelBase, GraphicsObject);
	public:
		ModelBase();
		ModelBase(const std::string& name);
		~ModelBase();

		static void RegisterObject();

		virtual void Load(const std::shared_ptr<DataStream>& dataStream);
		
		virtual void Destroy();

		virtual void renderObjects();

		virtual void createHw() override;
		virtual void ReleaseHw() override;

		void saveBinary(const std::string& filename);

		std::vector<SubMesh*>& getSubmehes() { return m_subMeshes; }

	private:
		std::vector<SubMesh*> m_subMeshes;
		std::string m_filename;
	};

#define MODELFILE_MAGIC (('\0' << 24) + ('L' << 16) + ('D' << 8) + 'M')
	
	//! Model file version constant
	const int kModelFileVersion = 1;

	//! Header of model file
	struct ModelFileHeader
	{
		//! see MODELFILE_MAGIC
		uint32_t magic;
		
		//! see kModelFileVersion
		uint32_t version;

		//! ModelFileSubmeshData enties count
		uint32_t submeshCount;
	};
	
	//! Data structure
	struct ModelFileSubmeshData
	{
		// Material info
		char materialInfo[248];

		// Vertex data
		uint32_t verticesCount;
		uint32_t indicesCount;
	};
	
	//! Model submesh class
	class ModelSubmesh : public GraphicsObject
	{
		IMPLEMENT_OBJECT(ModelSubmesh, GraphicsObject);
	public:
		static void RegisterObject();

		//! NOTE: The file pointer must be set before any ModelFileSubmeshData entries!
		void load(DataStreamPtr stream);
		void Save(DataStreamPtr stream);

		void createHw() override;
		void ReleaseHw() override;

		// Accessors
		IBufferBase* getVerticesBuffer() const { return m_verticesBuffer; }
		IBufferBase* getIndicesBuffer() const { return m_indicesBuffer; }
		const uint32_t getVerticesCount() const { return m_verticesCount; }
		const uint32_t getIndicesCount() const { return m_indicesCount; }
		std::weak_ptr<Material> getMaterial() const { return m_material; }

	private:
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;

		std::weak_ptr<Material> m_material;

		std::string m_materialName;

		IBufferBase* m_verticesBuffer;
		IBufferBase* m_indicesBuffer;

		uint32_t m_verticesCount;
		uint32_t m_indicesCount;

	};

	//! Model class
	class Model : public GraphicsObject
	{
		IMPLEMENT_OBJECT(Model, GraphicsObject);
	public:
		static void RegisterObject();
		
	public:
		Model();
		~Model();

		void Load(const std::shared_ptr<DataStream>& stream) override;
		void Save(const std::shared_ptr<DataStream>& stream) override;

		void createHw() override;
		void ReleaseHw() override;

	private:
		std::vector<std::shared_ptr<ModelSubmesh>> m_submeshes;

	};

	std::string getDirectoryPath(const std::string& filepath);
	void osConvertPath(std::string& path);
	void osConvertStandartPath(std::string& path);
}

#endif // !MODEL_H
