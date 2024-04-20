#include "graphicspch.h"
#include "graphics/renderer.h"

// Core module
#include "graphics/core/device.h"

// Managers
#include "graphics/ShaderProgramManager.h"
#include "graphics/shaderconstantmanager.h"

// Objects
#include "graphics/viewsurface.h"

namespace engine
{

Renderer* g_renderer = nullptr;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
	// Initialize render device

	extern IRenderDevice* createRenderDevice();
	g_renderDevice = createRenderDevice();
	
	Assert(g_renderDevice);

	// Initialize managers ...

	// Initialize shader manager

	extern IShaderProgramManager* createShaderProgramManager();
	g_shaderManager = createShaderProgramManager();

	// Initialize constant manager
	ShaderConstantManager::getInstance()->init();

}

void Renderer::shutdown()
{
	// Shutdown constant manager
	ShaderConstantManager::getInstance()->shutdown();

	// release shader manager
	if (g_shaderManager)
	{
		g_shaderManager->shutdown();

		mem_delete(g_shaderManager);
		g_shaderManager = nullptr;
	}

	// Release device
	if (g_renderDevice)
	{
		mem_delete(g_renderDevice);
		g_renderDevice = nullptr;
	}
}

void Renderer::beginFrame()
{
	
}

void Renderer::endFrame()
{

}

void Renderer::renderView(IViewSurface* view, int frameSyncNum)
{
	view->display(frameSyncNum);
}

IViewSurface* Renderer::createViewSurface(void* handle)
{
	extern IViewSurface* createPlatfromViewSurface(void* handle);
	return createPlatfromViewSurface(handle);
}

IRenderDevice* Renderer::getRenderDevice()
{
	Assert2(g_renderDevice, "Render device is not initialized.");
	return g_renderDevice;
}

}
