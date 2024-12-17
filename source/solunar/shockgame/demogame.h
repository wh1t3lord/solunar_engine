#ifndef DEMOGAME_H
#define DEMOGAME_H

#include "engine/entity/logiccomponent.h"

#include "engine/camera.h"

namespace solunar
{

class UsableAreaComponent : public LogicComponent
{
	DECLARE_OBJECT(UsableAreaComponent);
	DECLARE_PROPERTY_REGISTER(UsableAreaComponent);
public:
	UsableAreaComponent();
	~UsableAreaComponent();

	void LoadXML(tinyxml2::XMLElement& element) override;
	void SaveXML(tinyxml2::XMLElement& element) override;

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
	DECLARE_OBJECT(EditorCameraComponent);
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

class FreeCameraComponent : public LogicComponent
{
	DECLARE_OBJECT(FreeCameraComponent);
public:
	FreeCameraComponent();
	~FreeCameraComponent();

	void OnEntitySet(Entity* entity) override;
	//void OnWorldSet(World* world) override;

	void OnEntityRemove() override;

	// Update component once per frame.
	void Update(float delta) override;

private:
	CameraFirstPersonComponent* m_camera;
};

void Debug_Draw3DText(const char* text, const glm::vec3& position, const glm::vec4& color, float y = 0.0f);

class GameManager
{
public:
	GameManager();
	~GameManager();

	void OnWorldLoad(const std::string& worldName, World* pLoadedWorld);
};

extern GameManager* g_GameManager;

extern Entity* g_Player;

class FadeRenderer
{
public:
	static FadeRenderer* GetInstance();

public:
	void SetFade(float time, bool isOut);

	void Draw();

private:
	float m_time;
	float m_currentTime;
	bool m_isOut;
};

class PlayerSpawnComponent : public Component
{
public:
	DECLARE_OBJECT(PlayerSpawnComponent);

public:
	PlayerSpawnComponent();
	~PlayerSpawnComponent();
};

}


#endif