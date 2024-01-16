#include "graphicspch.h"
#include "graphics/graphics.h"

#include "graphics/graphicsobject.h"
#include "graphics/texturemap.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/model.h"

#include "graphics/renderer.h"

namespace engine
{

void registerGraphicsObjects()
{
	g_typeManager->registerObject<GraphicsObject>();
	g_typeManager->registerObject<TextureMap>();
	g_typeManager->registerObject<Material>();
	g_typeManager->registerObject<GenericMaterial>();
	g_typeManager->registerObject<LayeredMaskedMaterial>();
	g_typeManager->registerObject<NullMaterial>();
	g_typeManager->registerObject<ModelBase>();
	g_typeManager->registerObject<Model>();
	g_typeManager->registerObject<ModelSubmesh>();
}

void graphicsInit()
{
	Core::msg("Initializing graphics");

	registerGraphicsObjects();

	createRenderer();
	g_renderer->init();
}

void graphicsShutdown()
{
	g_renderer->shutdown();
	destroyRenderer();
}

}