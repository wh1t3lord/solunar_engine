#include "graphicspch.h"
#include "graphics/graphics.h"
#include "graphics/renderer.h"

namespace engine
{

void registerGraphicsObjects()
{
	// Graphics objects
}

void graphicsInit()
{
	Core::msg("Initializing graphics");

	registerGraphicsObjects();

	g_renderer = mem_new<Renderer>();
	g_renderer->init();
}

void graphicsShutdown()
{
	Core::msg("Shutdown graphics");

	if (g_renderer)
	{
		g_renderer->shutdown();
		mem_delete(g_renderer);
		g_renderer = nullptr;
	}
}

}