#include "shockgamepch.h"
#include "shockgame/demogame.h"

#include "graphics/ifontmanager.h"

#include "engine/engine.h"
#include "engine/inputmanager.h"
#include "engine/camera.h"

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

	g_engineData.m_shouldCaptureMouse = true;
	g_engineData.m_shouldHideMouse = true;
}

void EditorCameraComponent::OnEntityRemove()
{
	LogicComponent::OnEntityRemove();
}

void EditorCameraComponent::Update(float delta)
{
	LogicComponent::Update(delta);

	InputManager* input = InputManager::GetInstance();
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
}

}
