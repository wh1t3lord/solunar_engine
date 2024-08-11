#include "graphicspch.h"
#include "graphics/lightmanager.h"
#include "graphics/light.h"
#include "graphics/debugrenderer.h"

#include <imgui.h>

namespace solunar
{
	void LightManager::addLight(LightComponent* light)
	{
		Assert(light);
		m_lights.push_back(light);

		if (PointLightComponent* pointLight = dynamicCast<PointLightComponent>(light))
			m_pointLights.push_back(pointLight);
	}

	void LightManager::removeLight(LightComponent* light)
	{
		Assert(light);

		auto it = std::find(m_lights.begin(), m_lights.end(), light);
		if (it != m_lights.end())
		{
			m_lights.erase(it);
		}

		// clean from point light array
		if (PointLightComponent* pointLight = dynamicCast<PointLightComponent>(light))
		{
			// find in array
			auto it2 = std::find(m_pointLights.begin(), m_pointLights.end(), pointLight);
			if (it2 != m_pointLights.end())
			{
				m_pointLights.erase(it2);
			}
		}
	}

	DirectionalLightComponent* LightManager::GetDirectionalLight()
	{
		for (auto it : m_lights)
		{
			if (it->IsA<DirectionalLightComponent>())
				return dynamicCast<DirectionalLightComponent>(it);
		}

		return nullptr;
	}

	void graphicsLightEditor(bool* open)
	{
		if (ImGui::Begin("Light Editor", open))
		{
			DirectionalLightComponent* directionalLight = nullptr;
			if (LightManager::GetInstance()) directionalLight = LightManager::GetInstance()->GetDirectionalLight();
			if (directionalLight)
			{
				Entity* entity = directionalLight->GetEntity();
				ImGui::Text("DirectionalLightComponent at entity 0x%p", entity);

				g_debugRender.drawBoundingBox(entity->GetBoundingBox(), glm::vec3(1.f, 1.f, 0.f));
			
				static glm::vec3 position = glm::vec3(0.f);
				ImGui::DragFloat3("Position", &position[0], 0.2f, -20.f, 20.f);
				entity->SetPosition(position);

#if 1
				static glm::vec3 rotation = glm::vec3(0.0f);
				ImGui::DragFloat3("Euler Rotation", &rotation[0], 1.0f, -360.f, 360.f);
				entity->SetEulerRotation(glm::radians(glm::vec3(rotation)));
#else
				
				ImGui::DragFloat3("Euler Rotation", &directionalLight->m_direction[0], 1.0f, -360.f, 360.f);
#endif
				static glm::vec4 color = glm::vec4(1.0f);
				ImGui::DragFloat4("Color (R, G, B, A)", &color[0], 0.1f, 0.f, 1.f);
				directionalLight->m_color = color;

				g_debugRender.PushModelMatrix(entity->GetWorldTranslation());
				g_debugRender.drawAxis(glm::vec3(0.0f));
				g_debugRender.PopModelMatrix();

			}
		}

		ImGui::End();
	}
}