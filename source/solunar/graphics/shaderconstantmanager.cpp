#include "graphicspch.h"
#include "graphics/shaderconstantmanager.h"

#include "core/timer.h"

#include "engine/engine.h"
#include "engine/camera.h"
#include "engine/entity/world.h"

#include "graphics/core/device.h"
#include "graphics/rendercontext.h"
#include "graphics/shaderprogram.h"

#include "graphics/graphicsworld.h"
#include "graphics/light.h"

namespace engine
{
	static ShaderConstantManager s_shaderConstantManager;
	ConstantBufferProxy g_staticMeshConstantBuffer;
	ConstantBufferProxy g_directionalLightConstantBuffer;

	ConstantBufferProxy::ConstantBufferProxy(IBufferBase* buffer)
	{
		Assert(buffer);
		m_buffer = buffer;
	}

	ConstantBufferProxy::ConstantBufferProxy(IBufferBase* buffer, const std::string& name)
	{
		Assert(buffer);
		Assert(name.empty());
		m_buffer = buffer;
		m_name = name;
	}

	ConstantBufferProxy::ConstantBufferProxy(const ConstantBufferProxy& proxy)
	{
		Assert(proxy.m_buffer);
		Assert(proxy.m_name.empty());
		m_buffer = proxy.m_buffer;
		m_name = proxy.m_name;
	}

	ConstantBufferProxy::~ConstantBufferProxy()
	{
		m_buffer = nullptr;
	}

	IBufferBase* ConstantBufferProxy::get()
	{
		return m_buffer;
	}

	IBufferBase* ConstantBufferProxy::operator->()
	{
		return m_buffer;
	}

	std::string ConstantBufferProxy::name()
	{
		return m_name;
	}

	ShaderConstantManager::ShaderConstantManager()
	{
	}

	ShaderConstantManager::~ShaderConstantManager()
	{
	}

	void ShaderConstantManager::init()
	{
		Core::msg("ShaderConstantManager: initialize constant buffers ...");

		g_staticMeshConstantBuffer = create<StaticMeshGlobalData>("StaticMeshGlobalData");
		g_directionalLightConstantBuffer = create<DirectionalLightCB>("DirectionalLightCB");
	}

	void ShaderConstantManager::shutdown()
	{
		for (auto it : m_constantBuffers)
		{
			Core::msg("ShaderConstantManager: releasing constant buffer %s", it.first.c_str());
			mem_delete(it.second);
		}

		m_constantBuffers.clear();
	}

	ConstantBufferProxy ShaderConstantManager::get(const std::string& name)
	{
		auto it = m_constantBuffers.find(name);
		if (it != m_constantBuffers.end())
		{
			return ConstantBufferProxy(it->second);
		}

		return ConstantBufferProxy(nullptr);
	}


	void ShaderConstantManager::setStaticMeshGlobalData(MeshComponent* meshComponent, View* view, RenderContext& renderContext, GraphicsWorld* graphicsWorld)
	{
		// OPTICK_EVENT("ShaderConstantManager::setStaticMeshGlobalData");

		StaticMeshGlobalData* globalData = (StaticMeshGlobalData*)g_staticMeshConstantBuffer->map(BufferMapping::WriteOnly);
		globalData->m_modelMatrix = renderContext.model;
		globalData->m_viewMatrix = renderContext.view;
		globalData->m_projectionMatrix = renderContext.proj;

		glm::mat4 modelViewProjection = glm::mat4(1.0f);
		modelViewProjection = renderContext.proj * renderContext.view * renderContext.model;

		globalData->m_modelViewProjection = modelViewProjection;

		Camera* camera = CameraProxy::getInstance();
		globalData->m_viewPos = glm::vec4(camera->getPosition(), 0.0f);
		globalData->m_viewDir = glm::vec4(camera->getDirection(), 0.0f);

		globalData->m_directionLightVec = glm::vec4(0.0f);
		globalData->m_directionLightColor = glm::vec4(0.0f);

#if 0
		for (auto& light : graphicsWorld->getLightManager()->getLights())
		{
			if (light->isA(DirectionalLightComponent::getStaticTypeInfo()))
			{
				DirectionalLightComponent* directionalLight = dynamicCast<DirectionalLightComponent>(light);
				globalData->m_directionLightVec = glm::vec4(directionalLight->getDirection(), 0.0f);
				globalData->m_directionLightColor = glm::vec4(directionalLight->getColor(), 0.0f);
			}
		}

		m_staticMeshConstantBuffer->unmap();

		g_renderDevice->setConstantBufferIndex(0, m_staticMeshConstantBuffer);
#else
		g_staticMeshConstantBuffer->unmap();
		g_renderDevice->setConstantBufferIndex(0, g_staticMeshConstantBuffer.get());
#endif

	}

}