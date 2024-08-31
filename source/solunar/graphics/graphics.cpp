#include "graphicspch.h"
#include "graphics/graphics.h"

#include "graphics/graphicsobject.h"
#include "graphics/texturemap.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/model.h"
#include "graphics/animatedmodel.h"
#include "graphics/light.h"

#include "graphics/renderer.h"

namespace solunar
{

void registerGraphicsObjects()
{
	// Graphics objects

	g_typeManager->RegisterObject<GraphicsObject>();
	g_typeManager->RegisterObject<TextureMap>();
	g_typeManager->RegisterObject<Material>();
	g_typeManager->RegisterObject<GenericMaterial>();
	g_typeManager->RegisterObject<LayeredMaskedMaterial>();
	g_typeManager->RegisterObject<NullMaterial>();
	g_typeManager->RegisterObject<ModelBase>();
	g_typeManager->RegisterObject<AnimatedModel>();
	g_typeManager->RegisterObject<Model>();
	g_typeManager->RegisterObject<ModelSubmesh>();

	// Components
	g_typeManager->RegisterObject<MeshComponent>();
	g_typeManager->RegisterObject<AnimatedMeshComponent>();

	LightComponent::RegisterObject();
	PointLightComponent::RegisterObject();
	DirectionalLightComponent::RegisterObject();
	SpotLightComponent::RegisterObject();
}

void graphicsInit()
{
	Core::Msg("Initializing graphics");

	registerGraphicsObjects();

	createRenderer();
	g_renderer->Init();
}

void graphicsShutdown()
{
	g_renderer->Shutdown();
	destroyRenderer();
}

}