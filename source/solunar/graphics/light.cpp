#include "graphicspch.h"
#include "engine/entity/entity.h"
#include "engine/entity/world.h"
#include "graphics/light.h"
#include "graphics/lightmanager.h"
#include "graphics/graphicsworld.h"

namespace solunar
{
	IMPLEMENT_OBJECT(LightComponent, Component);

	BEGIN_PROPERTY_REGISTER(LightComponent)
	{
		REGISTER_PROPERTY(LightComponent, PropertyVector3, m_color);
		REGISTER_PROPERTY(LightComponent, PropertyVector3, m_ambientColor);
		REGISTER_PROPERTY(LightComponent, PropertyVector3, m_specularColor);
		REGISTER_PROPERTY(LightComponent, PropertyFloat, m_radius);
	}
	END_PROPERTY_REGISTER(LightComponent)

	LightComponent::LightComponent()
	{
		m_color = glm::vec3(1.0f);
		m_ambientColor = glm::vec3(1.0f);
		m_specularColor = glm::vec3(1.0f);
	//	m_shininess = 128;
		m_radius = 12.0f;
	}

	LightComponent::~LightComponent()
	{
	}

	void LightComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<LightComponent>();
	}

	void LightComponent::OnEntitySet(Entity* entity)
	{
		Component::OnEntitySet(entity);

		World* world = GetWorld();
		if (world)
		{
			if (GraphicsWorld* gfxWorld = world->GetGraphicsWorld())
			{
				gfxWorld->GetLightManager()->AddLight(this);
			}
		}
	}

	void LightComponent::OnEntityRemove()
	{
		World* world = GetWorld();
		if (world)
		{
			if (GraphicsWorld* gfxWorld = world->GetGraphicsWorld())
			{
				gfxWorld->GetLightManager()->RemoveLight(this);
			}
		}
		
		Component::OnEntityRemove();
	}

	void LightComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		Component::LoadXML(element);

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

	void LightComponent::SaveXML(tinyxml2::XMLElement& element)
	{
		Component::SaveXML(element);

		tinyxml2::XMLElement* ambientColorElement = element.InsertNewChildElement("AmbientColor");
		ambientColorElement->SetAttribute("r", m_ambientColor.r);
		ambientColorElement->SetAttribute("g", m_ambientColor.g);
		ambientColorElement->SetAttribute("b", m_ambientColor.b);

		tinyxml2::XMLElement* colorElement = element.InsertNewChildElement("Color");
		colorElement->SetAttribute("r", m_color.r);
		colorElement->SetAttribute("g", m_color.g);
		colorElement->SetAttribute("b", m_color.b);

		tinyxml2::XMLElement* specularColorElement = element.InsertNewChildElement("SpecularColor");
		specularColorElement->SetAttribute("r", m_specularColor.r);
		specularColorElement->SetAttribute("g", m_specularColor.g);
		specularColorElement->SetAttribute("b", m_specularColor.b);
	}

	/////////////////////////////////////////////////////////////////////
	// Point Light

	IMPLEMENT_OBJECT(PointLightComponent, LightComponent);

	void PointLightComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<PointLightComponent>();
	}

	void PointLightComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		LightComponent::LoadXML(element);

		tinyxml2::XMLElement* radiusElement = element.FirstChildElement("Radius");
		Assert(radiusElement && "Point light require radius element!");

		const tinyxml2::XMLAttribute* radiusValue = radiusElement->FindAttribute("value");
		Assert(radiusValue && "Point light require value attribute in Radius element!");

		m_radius = radiusValue->FloatValue();
	}

	void PointLightComponent::SaveXML(tinyxml2::XMLElement& element)
	{
		LightComponent::SaveXML(element);
	
		tinyxml2::XMLElement* radiusElement = element.InsertNewChildElement("Radius");
		radiusElement->SetAttribute("value", m_radius);
	}


	/////////////////////////////////////////////////////////////////////
	// Directional Light

	IMPLEMENT_OBJECT(DirectionalLightComponent, LightComponent);

	BEGIN_PROPERTY_REGISTER(DirectionalLightComponent)
	{
		REGISTER_PROPERTY(DirectionalLightComponent, PropertyVector3, m_direction);
	}
	END_PROPERTY_REGISTER(DirectionalLightComponent)

	void DirectionalLightComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<DirectionalLightComponent>();
	}

	/////////////////////////////////////////////////////////////////////
	// Spot Light

	IMPLEMENT_OBJECT(SpotLightComponent, LightComponent);

	BEGIN_PROPERTY_REGISTER(SpotLightComponent)
	{
		REGISTER_PROPERTY(SpotLightComponent, PropertyFloat, m_cutoff);
	}
	END_PROPERTY_REGISTER(SpotLightComponent)

	void SpotLightComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<SpotLightComponent>();
	}

	void SpotLightComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		LightComponent::LoadXML(element);

		tinyxml2::XMLElement* cutoffElement = element.FirstChildElement("Cutoff");
		Assert(cutoffElement && "Spot light require cutoff element!");

		const tinyxml2::XMLAttribute* cutoffValue = cutoffElement->FindAttribute("value");
		Assert(cutoffValue && "Spot light require value attribute in cutoff element!");

		m_cutoff = cutoffValue->FloatValue();
	}

	void SpotLightComponent::SaveXML(tinyxml2::XMLElement& element)
	{
	}

}
