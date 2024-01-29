#include "graphicspch.h"
#include "graphics/mesh.h"
#include "graphics/debugrenderer.h"

namespace engine {

	void MeshComponent::registerObject()
	{
		g_typeManager->registerObject<MeshComponent>();
	}

	MeshComponent::MeshComponent()
	{
	}

	MeshComponent::~MeshComponent()
	{
	}

	void MeshComponent::loadXML(tinyxml2::XMLElement& element)
	{
		
	}

	void MeshComponent::saveXML(tinyxml2::XMLElement& element)
	{
	}

	void MeshComponent::render()
	{
		BoundingBox boundingBox = getEntity()->getBoundingBox();
		g_debugRender.drawBoundingBox(boundingBox, glm::vec3(1.f));
	}
}