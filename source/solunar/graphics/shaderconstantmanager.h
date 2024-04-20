#ifndef SHADERCONSTANTMANAGER_H
#define SHADERCONSTANTMANAGER_H

#include "graphics/view.h"
#include "graphics/rendercontext.h"

namespace engine
{
	class IShaderProgram;
	class MeshComponent;

	enum class ShaderConstantType
	{
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Matrix3x3,
//		Matrix3x4,
		Matrix4x4,
	};

	struct StaticMeshGlobalData
	{
		glm::mat4 m_modelMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_modelViewProjection;

		glm::vec4 m_viewPos;
		glm::vec4 m_viewDir;
	};

	class ConstantBufferProxy
	{
	public:
		ConstantBufferProxy() = default;
		ConstantBufferProxy(IBufferBase* buffer);
		ConstantBufferProxy(IBufferBase* buffer, const std::string& name);
		ConstantBufferProxy(const ConstantBufferProxy& proxy);
		
		~ConstantBufferProxy();

		IBufferBase* get();
		IBufferBase* operator->();

		std::string name();

	private:
		std::string m_name;
		IBufferBase* m_buffer = nullptr;
	};

	// \brief Shader Constant Manager, manage the constant buffers.
	class ShaderConstantManager : public Singleton<ShaderConstantManager>
	{
	public:
		ShaderConstantManager();
		~ShaderConstantManager();

		// Initialize constant manager
		void init();

		// Shutdown constant manager
		void shutdown();

		// Create tagged constant buffer
		template <typename BufferStructureData>
		ConstantBufferProxy create(const std::string& name);
	
		// Get tagged constant buffer
		ConstantBufferProxy get(const std::string& name);

	private:
		std::unordered_map<std::string, IBufferBase*> m_constantBuffers;
	};

	extern ConstantBufferProxy g_staticMeshConstantBuffer;

	template<typename BufferStructureData>
	inline ConstantBufferProxy ShaderConstantManager::create(const std::string& name)
	{
		// Create constant buffer

		BufferDesc bufferDesc = {};
		bufferDesc.m_bufferType = BufferType::ConstantBuffer;
		bufferDesc.m_bufferAccess = BufferAccess::Stream;
		bufferDesc.m_bufferMemorySize = sizeof(BufferStructureData);

		SubresourceDesc subresourceDesc = {};

		IBufferBase* buffer = g_renderDevice->createBuffer(bufferDesc, subresourceDesc);
		m_constantBuffers.emplace(name, buffer);
		return ConstantBufferProxy(buffer);
	}
}

#endif // !SHADERCONSTANTMANAGER_H
