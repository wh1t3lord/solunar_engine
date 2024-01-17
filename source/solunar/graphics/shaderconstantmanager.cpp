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

namespace engine
{
	static ShaderConstantManager s_shaderConstantManager;

	ShaderConstantManager::ShaderConstantManager() :
		m_staticMeshConstantBuffer(nullptr)
	{
	}

	ShaderConstantManager::~ShaderConstantManager()
	{
	}

	void ShaderConstantManager::init()
	{
		Core::msg("[graphics]: initialize constant buffers ...");

		// Create constant buffer for static mesh

		BufferDesc bufferDesc;
		memset(&bufferDesc, 0, sizeof(bufferDesc));
		bufferDesc.m_bufferType = BufferType::ConstantBuffer;
		bufferDesc.m_bufferAccess = BufferAccess::Stream;
		bufferDesc.m_bufferMemorySize = sizeof(StaticMeshGlobalData);

		SubresourceDesc subresourceDesc;
		memset(&subresourceDesc, 0, sizeof(subresourceDesc));

		m_staticMeshConstantBuffer = g_renderDevice->createBuffer(bufferDesc, subresourceDesc);
	}

	void ShaderConstantManager::shutdown()
	{
		if (m_staticMeshConstantBuffer)
		{
			mem_delete(m_staticMeshConstantBuffer);
			m_staticMeshConstantBuffer = nullptr;
		}
	}

	void ShaderConstantManager::setStaticMeshGlobalData(StaticMeshComponent* meshComponent, View* view, RenderContext& renderContext, GraphicsWorld* graphicsWorld)
	{
		// OPTICK_EVENT("ShaderConstantManager::setStaticMeshGlobalData");
#if 0
		StaticMeshGlobalData* globalData = (StaticMeshGlobalData*)m_staticMeshConstantBuffer->map(BufferMapping::WriteOnly);
		globalData->m_modelMatrix = renderContext.model;
		globalData->m_viewMatrix = renderContext.view;
		globalData->m_projectionMatrix = renderContext.proj;

		glm::mat4 modelViewProjection = glm::mat4(1.0f);
		modelViewProjection = renderContext.proj * renderContext.view * renderContext.model;

		globalData->m_modelViewProjection = modelViewProjection;

		CameraProxy* cameraProxy = CameraProxy::getInstance();
		globalData->m_viewPos = glm::vec4(cameraProxy->getPosition(), 0.0f);
		globalData->m_viewDir = glm::vec4(cameraProxy->getDirection(), 0.0f);

		globalData->m_directionLightVec = glm::vec4(0.0f);
		globalData->m_directionLightColor = glm::vec4(0.0f);

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
#endif
	}
}