#ifndef MODEL_H
#define MODEL_H

#include "core/math/boundingbox.h"
#include "graphics/graphicsobject.h"
#include "graphics/vertexformat.h"

namespace engine
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

	extern VertexFormat s_vfVertex;

	class ModelBase;
	class Material;
	class IBufferBase;

	IBufferBase* createVBFromModel(VertexFormat* meshVF, int countOfVertexDecls, const std::string& filename, int* verticesCount);

	class SubMesh
	{
		friend class ModelBase;
	public:
		SubMesh();
		SubMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);
		void load(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);

		~SubMesh();

		void destroy();

		void render();

		IBufferBase* getVertexBuffer() { return m_vertexBuffer; }
		IBufferBase* getIndexBuffer() { return m_indexBuffer; }

		const uint32_t getVerticesCount() const { return m_verticesCount; }
		const uint32_t getIndeciesCount() const { return m_indeciesCount; }

		const glm::mat4 getTransform() const { return m_transform; }

		std::weak_ptr<Material> getMaterial() { return m_material; }

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
		ImplementObject(ModelBase, GraphicsObject);
	public:
		ModelBase();
		ModelBase(const std::string& name);
		~ModelBase();

		static void registerObject();

		virtual void load(const std::shared_ptr<DataStream>& dataStream);
		
		virtual void destroy();

		virtual void renderObjects();

		void createHw() override;
		void releaseHw() override;

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
		ImplementObject(ModelSubmesh, GraphicsObject);
	public:
		static void registerObject();

		//! NOTE: The file pointer must be set before any ModelFileSubmeshData entries!
		void load(DataStreamPtr stream);
		void save(DataStreamPtr stream);

		void createHw() override;
		void releaseHw() override;

		// Accessors
		IBufferBase* getVerticesBuffer() const { return m_verticesBuffer; }
		IBufferBase* getIndicesBuffer() const { return m_indicesBuffer; }
		const size_t getVerticesCount() const { return m_verticesCount; }
		const size_t getIndicesCount() const { return m_indicesCount; }
		std::weak_ptr<Material> getMaterial() const { return m_material; }

	private:
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;

		std::weak_ptr<Material> m_material;

		std::string m_materialName;

		IBufferBase* m_verticesBuffer;
		IBufferBase* m_indicesBuffer;

		size_t m_verticesCount;
		size_t m_indicesCount;

	};

	//! Model class
	class Model : public GraphicsObject
	{
		ImplementObject(Model, GraphicsObject);
	public:
		static void registerObject();
		
	public:
		Model();
		~Model();

		void load(const std::shared_ptr<DataStream>& stream) override;
		void save(const std::shared_ptr<DataStream>& stream) override;

		void createHw() override;
		void releaseHw() override;

	private:
		std::vector<std::shared_ptr<ModelSubmesh>> m_submeshes;

	};
}

#endif // !MODEL_H
