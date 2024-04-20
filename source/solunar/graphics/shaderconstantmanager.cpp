#include "graphicspch.h"
#include "graphics/shaderconstantmanager.h"

#include "core/timer.h"

#include "engine/engine.h"
#include "engine/camera.h"
#include "engine/entity/world.h"

#include "graphics/core/device.h"
#include "graphics/rendercontext.h"
#include "graphics/shaderprogram.h"

namespace engine
{
	static ShaderConstantManager s_shaderConstantManager;
	ConstantBufferProxy g_staticMeshConstantBuffer;

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

}