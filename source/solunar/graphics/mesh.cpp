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
		m_boundingBox.setIdentity();
	}

	MeshComponent::~MeshComponent()
	{
	}

	void MeshComponent::loadXML(tinyxml2::XMLElement& element)
	{
		
	}

	void MeshComponent::saveXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* bbox = element.InsertNewChildElement("BoundingBox");
		if (bbox)
		{
			tinyxml2::XMLElement* bboxMin = bbox->InsertNewChildElement("Min");
			bboxMin->SetAttribute("x", m_boundingBox.m_min.x);
			bboxMin->SetAttribute("y", m_boundingBox.m_min.y);
			bboxMin->SetAttribute("z", m_boundingBox.m_min.z);

			tinyxml2::XMLElement* bboxMax = bbox->InsertNewChildElement("Max");
			bboxMax->SetAttribute("x", m_boundingBox.m_max.x);
			bboxMax->SetAttribute("y", m_boundingBox.m_max.y);
			bboxMax->SetAttribute("z", m_boundingBox.m_max.z);
		}
	}

	void MeshComponent::render()
	{
		g_debugRender.DrawBoundingBox(m_boundingBox, glm::vec3(1.f));
	}
}