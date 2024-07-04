#ifndef LIGHT_H
#define LIGHT_H

#include "engine/entity/component.h"
#include "graphics/lightmanager.h"

namespace engine
{
	#define MAX_POINT_LIGHTS 32

	struct DirectionalLightCB
	{
		glm::vec4 m_direction;
		glm::vec4 m_ambientColor;
		glm::vec4 m_color;
	};

	struct PointLightCB
	{
		glm::vec4 position;

		// colors
		glm::vec4 color;
		glm::vec4 specular;

		glm::vec4 lightData;
	};

	class LightComponent : public Component
	{
		ImplementObject(LightComponent, Component);
	public:
		LightComponent();
		~LightComponent();

		static void registerObject();
		
		void onEntitySet(Entity* entity) override;
		void onEntityRemove() override;

		virtual void loadXML(tinyxml2::XMLElement& element);
		virtual void saveXML(tinyxml2::XMLElement& element);

		glm::vec3 m_color;
		glm::vec3 m_ambientColor;
		glm::vec3 m_specularColor;
		float m_radius;
		
		//float m_fallofd;
		
	};

	class PointLightComponent : public LightComponent
	{
		ImplementObject(PointLightComponent, LightComponent);
	public:
		static void registerObject();

		void loadXML(tinyxml2::XMLElement& element) override;
		void saveXML(tinyxml2::XMLElement& element) override;
	};

	class DirectionalLightComponent : public LightComponent
	{
		ImplementObject(DirectionalLightComponent, LightComponent);
	public:
		static void registerObject();

		glm::vec3 m_direction;
	};
}

#endif // !LIGHT_H
