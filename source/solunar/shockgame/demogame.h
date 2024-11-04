#ifndef DEMOGAME_H
#define DEMOGAME_H

#include "engine/entity/logiccomponent.h"

#include "engine/camera.h"

namespace solunar
{

class UsableAreaComponent : public LogicComponent
{
	DECLARE_OBJECT(UsableAreaComponent);
public:
	UsableAreaComponent();
	~UsableAreaComponent();

	void LoadXML(tinyxml2::XMLElement& element) override;

	// Update component once per frame.
	void Update(float delta) override;


	void OnInit();

	const std::string& GetScriptName() { return m_scriptName; }

private:
	std::string m_scriptName;
	bool m_isInited;
};

class EditorCameraComponent : public LogicComponent
{
	DECLARE_OBJECT(EditorCameraComponent, LogicComponent);
public:
	EditorCameraComponent();
	~EditorCameraComponent();

	void OnEntitySet(Entity* entity) override;
	//void OnWorldSet(World* world) override;

	void OnEntityRemove() override;

	// Update component once per frame.
	void Update(float delta) override;

private:
	CameraFirstPersonComponent* m_camera;
};

void Debug_Draw3DText(const char* text, const glm::vec3& position, const glm::vec4& color);

class GameManager
{
public:
	GameManager();
	~GameManager();

	void OnWorldLoad(const std::string& worldName);
};

extern GameManager* g_GameManager;

extern Entity* g_Player;

}


#endif