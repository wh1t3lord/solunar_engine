#include "shockgamepch.h"
#include "shockgame/demogame.h"

#include "graphics/ifontmanager.h"

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

}
