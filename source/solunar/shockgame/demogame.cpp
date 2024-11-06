#include "shockgamepch.h"
#include "shockgame/demogame.h"

#include "engine/engine.h"
#include "engine/inputmanager.h"
#include "engine/camera.h"

#include "graphics/ifontmanager.h"
#include "graphics/debugrenderer.h"

#include "graphics/light.h"
#include "graphics/mesh.h"

#include <imgui.h>
#include <ImGuizmo.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp> // Matrix decomposition

namespace solunar
{

IMPLEMENT_OBJECT(UsableAreaComponent, LogicComponent);

UsableAreaComponent::UsableAreaComponent()
{
	m_isInited = false;
}

UsableAreaComponent::~UsableAreaComponent()
{
}

void UsableAreaComponent::LoadXML(tinyxml2::XMLElement& element)
{
	const tinyxml2::XMLElement* scriptElement = element.FirstChildElement("Script");
	if (scriptElement)
	{
		const char* scriptName = nullptr;
		tinyxml2::XMLError error = scriptElement->QueryStringAttribute("value", &scriptName);
		if (error == tinyxml2::XML_SUCCESS)
			m_scriptName = scriptName;
	}
}

void UsableAreaComponent::OnInit()
{
}

void UsableAreaComponent::Update(float delta)
{
	if (!m_isInited)
	{
		OnInit();

		m_isInited = true;
	}

	static char buf[256];
	snprintf(buf, sizeof(buf), "UsableAreaComponent: Script: %s", m_scriptName.c_str());

	Debug_Draw3DText(buf, GetEntity()->GetWorldPosition(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Debug_Draw3DText(const char* text, const glm::vec3& position, const glm::vec4& color)
{
	View* view = CameraProxy::GetInstance()->GetView();

	glm::vec4 vp = glm::vec4(0.0f, 0.0f, (float)view->m_width, (float)view->m_height);
	glm::vec3 proj = glm::project(position, view->m_view, view->m_projection, vp);
	if (proj.z >= 1.0f)
		return; // clip

	proj.y = ((float)view->m_height - 1.0f - proj.y);

	g_fontManager->DrawSystemFontShadowed(text, proj.x, proj.y, color);
}

void Debug_Draw3DText_Y(const char* text, const glm::vec3& position, const glm::vec4& color, float y = 0.0f)
{
	View* view = CameraProxy::GetInstance()->GetView();

	glm::vec4 vp = glm::vec4(0.0f, 0.0f, (float)view->m_width, (float)view->m_height);
	glm::vec3 proj = glm::project(position, view->m_view, view->m_projection, vp);
	if (proj.z >= 1.0f)
		return; // clip

	proj.y = ((float)view->m_height - 1.0f - proj.y) + y;

	g_fontManager->DrawSystemFontShadowed(text, proj.x, proj.y, color);
}


static GameManager s_GameManager;
GameManager* g_GameManager = &s_GameManager;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::OnWorldLoad(const std::string& worldName)
{
	Core::Msg("GameManager: Load script for level %s", worldName.c_str());

}

Entity* g_Player = nullptr;

Entity* g_SelectedEntity = nullptr;

IMPLEMENT_OBJECT(EditorCameraComponent, LogicComponent);

EditorCameraComponent::EditorCameraComponent() :
	m_camera(nullptr)
{
}

EditorCameraComponent::~EditorCameraComponent()
{
}

void EditorCameraComponent::OnEntitySet(Entity* entity)
{
	LogicComponent::OnEntitySet(entity);

	m_camera = entity->CreateComponent<CameraFirstPersonComponent>();
	
	CameraProxy::GetInstance()->SetCameraComponent(m_camera);

	g_engineData.m_shouldCaptureMouse = false;
	g_engineData.m_shouldHideMouse = false;
}

void EditorCameraComponent::OnEntityRemove()
{
	LogicComponent::OnEntityRemove();
}

inline glm::vec3 Glm_GetPositionFromMat4(const glm::mat4& m)
{
	glm::vec3 scale = glm::vec3(0.0f);
	glm::quat orientation = glm::identity<glm::quat>();
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 skew = glm::vec3(0.0f);
	glm::vec4 persp = glm::vec4(0.0f);
	glm::decompose(m, scale, orientation, position, skew, persp);
	return position;
}

void DrawProperties(const std::vector<IProperty*>& properties, Object* object)
{
	for (int i = 0; i < properties.size(); i++)
	{
		IProperty* property = properties[i];
		if (property->GetType() == PropertyType_Float)
		{
			float value;
			PropertyGetValue(object, property, value);
			ImGui::DragFloat(property->GetName(), &value);
		}
		if (property->GetType() == PropertyType_Vector3)
		{
			glm::vec3 value;
			PropertyGetValue(object, property, value);
			ImGui::DragFloat3(property->GetName(), &value[0]);
		}
		if (property->GetType() == PropertyType_Vector4)
		{
			glm::vec4 value;
			PropertyGetValue(object, property, value);
			ImGui::DragFloat4(property->GetName(), &value[0]);
		}
		if (property->GetType() == PropertyType_Quaternion)
		{
			glm::quat value;
			PropertyGetValue(object, property, value);
			ImGui::DragFloat4(property->GetName(), &value[0]);
		}
		if (property->GetType() == PropertyType_String)
		{
			std::string value;
			PropertyGetValue(object, property, value);
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), value.c_str());
		}
	}
}

void DrawEntityPropertyWindow(Entity* entity)
{
	if (ImGui::Begin("Entity Properties"))
	{
		if (entity)
		{
			ImGui::Text("%s", entity->GetTypeInfo()->GetClassName());
			std::vector<IProperty*> properties;
			PropertyManager::GetInstance()->GetProperties(entity->GetTypeInfo(), properties);
			DrawProperties(properties, entity);

			// components
			std::vector<Component*> components = entity->GetAllComponents();
			for (int i = 0; i < components.size(); i++)
			{
				ImGui::Separator();
				ImGui::Text("%s", components[i]->GetTypeInfo()->GetClassName());

				properties.clear();

				PropertyManager::GetInstance()->GetProperties(components[i]->GetTypeInfo(), properties);

				DrawProperties(properties, components[i]);
			}
		}
	}

	ImGui::End();
}

void EditorCameraComponent::Update(float delta)
{
	static std::vector<std::pair<glm::vec3, glm::vec3>> debugRays;

	LogicComponent::Update(delta);

	//if (!g_SelectedEntity)
	//{
	//	std::vector<Entity*> lights = Engine::ms_world->GetEntityManager().GetEntitiesWithComponent<PointLightComponent>();
	//	if (!lights.empty())
	//		g_SelectedEntity = lights[0];
	//}

	InputManager* input = InputManager::GetInstance();
	if (input->IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		g_engineData.m_shouldCaptureMouse = true;
		g_engineData.m_shouldHideMouse = true;

		glm::vec2 mousePos = input->GetCursorPos();

		glm::vec2 deltaMousePos = input->GetDeltaCursorPos();
		m_camera->updateFromMousePosition(deltaMousePos);

		glm::vec3 cameraDirection = CameraProxy::GetInstance()->GetDirection();
		glm::vec3 pos = GetEntity()->GetPosition();
		float camSpeed = 8.0f * delta;

		if (input->IsPressed(KeyboardKeys::KEY_LEFT_SHIFT))
			camSpeed = 18.0f * delta;

		if (input->IsPressed(KeyboardKeys::KEY_W))
			pos += camSpeed * cameraDirection;
		if (input->IsPressed(KeyboardKeys::KEY_S))
			pos -= camSpeed * cameraDirection;

		if (input->IsPressed(KeyboardKeys::KEY_Q))
			pos -= glm::normalize(glm::cross(cameraDirection, glm::vec3(1.0f, 0.0f, 0.0f))) * camSpeed;
		if (input->IsPressed(KeyboardKeys::KEY_E))
			pos += glm::normalize(glm::cross(cameraDirection, glm::vec3(1.0f, 0.0f, 0.0f))) * camSpeed;

		if (input->IsPressed(KeyboardKeys::KEY_A))
			pos -= glm::normalize(glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;
		if (input->IsPressed(KeyboardKeys::KEY_D))
			pos += glm::normalize(glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;

		GetEntity()->SetPosition(pos);
	
		// ray pick

		if (input->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Camera* camera = CameraProxy::GetInstance();
			glm::vec3 rayBegin = camera->GetPosition() + camera->GetDirection();
			glm::vec3 rayEnd = camera->GetPosition() + (camera->GetDirection() * 100.0f);
			RayCastResult rayCastResult;
			debugRays.emplace_back(std::make_pair(rayBegin, rayEnd));
			if (GetWorld()->RayCast(rayCastResult, rayBegin, rayEnd))
			{
				g_SelectedEntity = rayCastResult.m_entity;
			}
			else
			{
				g_SelectedEntity = nullptr;
			}
		}
	}
	else
	{
		g_engineData.m_shouldCaptureMouse = false;
		g_engineData.m_shouldHideMouse = false;

#if 0
		// ray pick
		
		if (input->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			glm::vec2 pos = input->GetCursorPos();
			glm::vec3 rayBegin = CameraProxy::GetInstance()->GetScreenRay(pos.x, pos.y);
			glm::vec3 rayEnd = rayBegin * -100.0f;
			RayCastResult rayCastResult;

			//debugRays.emplace_back(std::make_pair(rayBegin, rayEnd));

			if (GetWorld()->RayCast(rayCastResult, rayBegin, rayEnd))
			{
				g_SelectedEntity = rayCastResult.m_entity;
			}
			else
			{
				g_SelectedEntity = nullptr;
			}
		}
#endif
	}

#if 0
	for (int i = 0; i < debugRays.size(); i++)
	{
		g_debugRender.DrawLine(debugRays[i].first, debugRays[i].second, glm::vec3(1.0f, 0.0f, 0.0f));
	}
#endif

	if (g_SelectedEntity)
	{
		//g_debugRender.drawBoundingBox(g_SelectedEntity->GetBoundingBox(), glm::vec3(0.0f, 0.0f, 1.0f));

		{
			char buf[256];
			snprintf(buf, sizeof(buf), "Entity: 0x%p", g_SelectedEntity);
			Debug_Draw3DText_Y(buf, g_SelectedEntity->GetWorldPosition(), glm::vec4(1.0f), 0.0f);

			bool isLight = !!g_SelectedEntity->GetComponent<LightComponent>();
			bool isMesh = !!g_SelectedEntity->GetComponent<MeshComponent>();

			if (isLight)
			{
				snprintf(buf, sizeof(buf), "Light");
				Debug_Draw3DText_Y(buf, g_SelectedEntity->GetWorldPosition(), glm::vec4(1.0f), 25.0f);
			}

			if (isMesh)
			{
				snprintf(buf, sizeof(buf), "Mesh");
				Debug_Draw3DText_Y(buf, g_SelectedEntity->GetWorldPosition(), glm::vec4(1.0f), 25.0f);
			}
		}

		static glm::mat4 objectMatrix = g_SelectedEntity->GetWorldTranslation();
		
		View* view = CameraProxy::GetInstance()->GetView();

		glm::mat4 cameraView = view->m_view;
		glm::mat4 cameraProj = view->m_projection;

		static int g_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		static int g_CurrentGizmoMode = ImGuizmo::WORLD;
		
		if (ImGuizmo::Manipulate(
			&cameraView[0][0],
			&cameraProj[0][0],
			(ImGuizmo::OPERATION)g_CurrentGizmoOperation,
			(ImGuizmo::MODE)g_CurrentGizmoMode,
			&objectMatrix[0][0]))
		{
			glm::vec3 pos = Glm_GetPositionFromMat4(objectMatrix);
			g_SelectedEntity->SetPosition(pos);

		}

	}

	DrawEntityPropertyWindow(g_SelectedEntity);
}

}
