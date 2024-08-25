#ifndef LIGHT_H
#define LIGHT_H

#include "engine/entity/component.h"
#include "graphics/lightmanager.h"

namespace solunar
{
	#define MAX_POINT_LIGHTS 32
	#define MAX_SPOT_LIGHTS 32

	struct DirectionalLightCB
	{
		glm::vec4 m_direction;
		glm::vec4 m_ambientColor;
		glm::vec4 m_color;
	};

	// #TODO: Encapsulate in to the one LightData 

	struct PointLightData
	{
		glm::vec4 position;

		// colors
		glm::vec4 color;
		glm::vec4 specular;

		glm::vec4 lightData;
	};

	struct SpotLightData
	{
		glm::vec4 position;

		// colors
		glm::vec4 color;
		glm::vec4 specular;

		glm::vec4 lightData;
	};

	struct PointLightCB
	{
		PointLightData pointLights[MAX_POINT_LIGHTS];
	};

	struct SpotLightCB
	{
		SpotLightData spotLights[MAX_SPOT_LIGHTS];
	};

	struct LightGlobalDataCB
	{
		uint32_t m_pointLightCount;
		uint32_t m_spotLightCount;

		uint32_t m_padding0;
		uint32_t m_padding1;
	};

	class LightComponent : public Component
	{
		DECLARE_OBJECT(LightComponent);
		DECLARE_PROPERTY_REGISTER(LightComponent);
	public:
		LightComponent();
		~LightComponent();

		static void RegisterObject();
		
		void OnEntitySet(Entity* entity) override;
		void OnEntityRemove() override;

		virtual void LoadXML(tinyxml2::XMLElement& element);
		virtual void SaveXML(tinyxml2::XMLElement& element);

		glm::vec3 m_color;
		glm::vec3 m_ambientColor;
		glm::vec3 m_specularColor;
		float m_radius;
		
		//float m_fallofd;
		
	};

	class PointLightComponent : public LightComponent
	{
		DECLARE_OBJECT(PointLightComponent);
	public:
		static void RegisterObject();

		void LoadXML(tinyxml2::XMLElement& element) override;
		void SaveXML(tinyxml2::XMLElement& element) override;
	};

	class DirectionalLightComponent : public LightComponent
	{
		DECLARE_OBJECT(DirectionalLightComponent);
		DECLARE_PROPERTY_REGISTER(DirectionalLightComponent);
	public:
		static void RegisterObject();

		glm::vec3 m_direction;
	};

	class SpotLightComponent : public LightComponent
	{
		DECLARE_OBJECT(SpotLightComponent);
		DECLARE_PROPERTY_REGISTER(SpotLightComponent);
	public:
		static void RegisterObject();

		void LoadXML(tinyxml2::XMLElement& element) override;
		void SaveXML(tinyxml2::XMLElement& element) override;

		float m_cutoff = 1.0f;
	};
}

#endif // !LIGHT_H
