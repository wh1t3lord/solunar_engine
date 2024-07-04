#include "graphicspch.h"
#include "graphics/lightmanager.h"
#include "graphics/light.h"
#include "graphics/debugrenderer.h"

#include <imgui.h>

namespace engine
{
	void LightManager::addLight(LightComponent* light)
	{
		Assert(light);
		m_lights.push_back(light);
	}

	void LightManager::removeLight(LightComponent* light)
	{
		Assert(light);
		
		typedef std::vector<LightComponent*>::iterator LT;

		LT I = m_lights.begin();
		LT E = m_lights.end();

		for (; I != E;)
			if (*I == light)
				break;

		m_lights.erase(I);
	}

	DirectionalLightComponent* LightManager::getDirectionalLight()
	{
		for (auto it : m_lights)
		{
			if (it->isA<DirectionalLightComponent>())
				return dynamicCast<DirectionalLightComponent>(it);
		}

		return nullptr;
	}

	void graphicsLightEditor(bool* open)
	{
		if (ImGui::Begin("Light Editor", open))
		{
			DirectionalLightComponent* directionalLight = LightManager::getInstance()->getDirectionalLight();
			if (directionalLight)
			{
				Entity* entity = directionalLight->getEntity();
				ImGui::Text("DirectionalLightComponent at entity 0x%p", entity);

				g_debugRender.drawBoundingBox(entity->getBoundingBox(), glm::vec3(1.f, 1.f, 0.f));
			
				static glm::vec3 position = glm::vec3(0.f);
				ImGui::DragFloat3("Position", &position[0], 0.2f, -20.f, 20.f);
				entity->setPosition(position);

#if 1
				static glm::vec3 rotation = glm::vec3(0.0f);
				ImGui::DragFloat3("Euler Rotation", &rotation[0], 1.0f, -360.f, 360.f);
				entity->setEulerRotation(glm::radians(glm::vec3(rotation)));
#else
				
				ImGui::DragFloat3("Euler Rotation", &directionalLight->m_direction[0], 1.0f, -360.f, 360.f);
#endif
				static glm::vec4 color = glm::vec4(1.0f);
				ImGui::DragFloat4("Color (R, G, B, A)", &color[0], 0.1f, 0.f, 1.f);
				directionalLight->m_color = color;

				g_debugRender.pushModelMatrix(entity->getWorldTranslation());
				g_debugRender.drawAxis(glm::vec3(0.0f));
				g_debugRender.popModelMatrix();

			}
		}

		ImGui::End();
	}
}