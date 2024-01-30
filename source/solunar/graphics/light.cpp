#include "graphicspch.h"
#include "engine/entity/entity.h"
#include "engine/entity/world.h"
#include "graphics/light.h"
#include "graphics/lightmanager.h"
#include "graphics/graphicsworld.h"

namespace engine
{
	LightComponent::LightComponent()
	{
		m_color = glm::vec3(2.0);
		m_ambientColor = glm::vec3(0.2);
		m_specularColor = glm::vec3(1.0);
	//	m_shininess = 128;
		m_radius = 5.0f;
	}

	LightComponent::~LightComponent()
	{
	}

	void LightComponent::registerObject()
	{
		g_typeManager->registerObject<LightComponent>();
	}

	void LightComponent::onEntitySet(Entity* entity)
	{
		Component::onEntitySet(entity);

		World* world = getWorld();
		if (world)
		{
			if (GraphicsWorld* gfxWorld = world->getGraphicsWorld())
			{
				gfxWorld->getLightManager()->addLight(this);
			}
		}
	}

	void LightComponent::onEntityRemove()
	{
		World* world = getWorld();
		if (world)
		{
			if (GraphicsWorld* gfxWorld = world->getGraphicsWorld())
			{
				gfxWorld->getLightManager()->removeLight(this);
			}
		}
		
		Component::onEntityRemove();
	}

	void LightComponent::loadXML(tinyxml2::XMLElement& element)
	{
		Component::loadXML(element);

		tinyxml2::XMLElement* ambientColorElement = element.FirstChildElement("AmbientColor");
		if (ambientColorElement)
		{
			const tinyxml2::XMLAttribute* r = ambientColorElement->FindAttribute("r");
			const tinyxml2::XMLAttribute* g = ambientColorElement->FindAttribute("g");
			const tinyxml2::XMLAttribute* b = ambientColorElement->FindAttribute("b");

			m_ambientColor.r = r->FloatValue();
			m_ambientColor.g = g->FloatValue();
			m_ambientColor.b = b->FloatValue();
		}

		tinyxml2::XMLElement* colorElement = element.FirstChildElement("Color");
		if (colorElement)
		{
			const tinyxml2::XMLAttribute* r = colorElement->FindAttribute("r");
			const tinyxml2::XMLAttribute* g = colorElement->FindAttribute("g");
			const tinyxml2::XMLAttribute* b = colorElement->FindAttribute("b");

			m_color.r = r->FloatValue();
			m_color.g = g->FloatValue();
			m_color.b = b->FloatValue();
		}


		tinyxml2::XMLElement* specularColorElement = element.FirstChildElement("SpecularColor");
		if (specularColorElement)
		{
			const tinyxml2::XMLAttribute* r = specularColorElement->FindAttribute("r");
			const tinyxml2::XMLAttribute* g = specularColorElement->FindAttribute("g");
			const tinyxml2::XMLAttribute* b = specularColorElement->FindAttribute("b");

			m_specularColor.r = r->FloatValue();
			m_specularColor.g = g->FloatValue();
			m_specularColor.b = b->FloatValue();
		}
	}

	void LightComponent::saveXML(tinyxml2::XMLElement& element)
	{
	}

	void PointLightComponent::registerObject()
	{
		g_typeManager->registerObject<PointLightComponent>();
	}

	void PointLightComponent::loadXML(tinyxml2::XMLElement& element)
	{
		LightComponent::loadXML(element);

		tinyxml2::XMLElement* radiusElement = element.FirstChildElement("Radius");
		Assert(radiusElement && "Point light require radius element!");

		const tinyxml2::XMLAttribute* radiusValue = radiusElement->FindAttribute("value");
		Assert(radiusValue && "Point light require value attribute in Radius element!");

		m_radius = radiusValue->FloatValue();
	}

	void PointLightComponent::saveXML(tinyxml2::XMLElement& element)
	{
	}

	void DirectionalLightComponent::registerObject()
	{
		g_typeManager->registerObject<DirectionalLightComponent>();
	}

}
