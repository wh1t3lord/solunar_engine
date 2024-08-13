#include "shockgamepch.h"

// Base game includes
#include "game/gamelogic/camera/cameramanager.h"
#include "game/gamelogic/mainmenu/mainmenuworldcomponent.h"
#include "game/gamelogic/player/playercontrollercomponent.h"
#include "game/gamelogic/player/weaponchaincomponent.h"
#include "game/gamelogic/saveload/saveloadmanager.h"
#include "game/gamelogic/weapons/weaponcomponent.h"
#include "game/gamelogic/weapons/weaponautocomponent.h"
#include "game/gamelogic/weapons/weaponpistolcomponent.h"
#include "game/gamelogic/world/levelmanager.h"

// ShockGame
#include "shockgame/shockgame.h"
#include "shockgame/shockeventlistener.h"
#include "shockgame/shocksignalmanager.h"
#include "shockgame/shockplayercontroller.h"
#include "shockgame/shockgameui.h"
#include "shockgame/shockaicomponent.h"
#include "shockgame/shockprojectilecomponent.h"

// Engine
#include "engine/camera.h"
#include "engine/engine.h"

// Graphics
#include "graphics/fontmanager.h"
#include "graphics/imguimanager.h"
#include "graphics/debugrenderer.h"
#include "graphics/fontmanager.h"

#undef min
#undef max

#include <numeric>

// Graphics Objects
#include "graphics/mesh.h"
#include "graphics/animatedmodel.h"

#include <array>
#include <algorithm>

#include "core/object/propertymanager.h"

namespace solunar
{

// implemetation for IGameInterface

static ShockGameInterface s_shockGameInterface;
IGameInterface* g_gameInterface = (IGameInterface*)&s_shockGameInterface;

ShockGameInterface* getShockGameInterface()
{
	return &s_shockGameInterface;
}

class TestRotatorComponent : public LogicComponent
{
	IMPLEMENT_OBJECT(TestRotatorComponent, LogicComponent);

public:
	TestRotatorComponent() : m_YAxis(0.0f)
	{}

	~TestRotatorComponent()
	{}

	static void RegisterObject()
	{
		g_typeManager->RegisterObject<TestRotatorComponent>();
	}

	void Update(float dt) override
	{
		m_YAxis += dt * 200.0f;
		GetEntity()->QuaternionRotate(glm::vec3(0.f, 1.f, 0.f), m_YAxis);
	}

private:
	float m_YAxis;
};

class TestPositionUpdaterComponent : public LogicComponent
{
	IMPLEMENT_OBJECT(TestPositionUpdaterComponent, LogicComponent);

public:
	TestPositionUpdaterComponent()
	{}

	~TestPositionUpdaterComponent()
	{}

	static void RegisterObject()
	{
		g_typeManager->RegisterObject<TestPositionUpdaterComponent>();
	}

	void Update(float dt) override
	{
		const float kSpeed = 2.0f;

		glm::vec3 position = GetEntity()->GetPosition();
		position.y += kSpeed * dt;
		GetEntity()->SetPosition(position);
	}

};

class TestAnimationComponent : public LogicComponent
{
	IMPLEMENT_OBJECT(TestAnimationComponent, LogicComponent);
public:
	TestAnimationComponent();
	~TestAnimationComponent();

	void LoadXML(tinyxml2::XMLElement& element) override;

	void Update(float dt) override;

private:
	std::string m_animationName;
	int m_animationIndex = -1;
};

TestAnimationComponent::TestAnimationComponent()
{
}

TestAnimationComponent::~TestAnimationComponent()
{
}

void TestAnimationComponent::LoadXML(tinyxml2::XMLElement& element)
{
	tinyxml2::XMLElement* animationNameElement = element.FirstChildElement("AnimationName");
	if (animationNameElement)
	{
		const tinyxml2::XMLAttribute* animationNameAttribute = animationNameElement->FindAttribute("name");
		if (animationNameAttribute)
		{
			m_animationName = animationNameAttribute->Value();
		}
	}
}

void TestAnimationComponent::Update(float dt)
{
	AnimatedMeshComponent* animatedMeshComponent = GetEntity()->GetComponent<AnimatedMeshComponent>();
	std::shared_ptr<ModelBase> modelBase = animatedMeshComponent->lockModel();
	AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(modelBase.get());
	if (animatedModel) {
		if (m_animationIndex == -1) {
			m_animationIndex = animatedModel->GetAnimationByName(m_animationName);
			animatedModel->PlayAnimation(m_animationIndex, true);
		}
	}

	animatedModel->Update(dt);
}

class ViewmodelAnimationController : public LogicComponent
{
	IMPLEMENT_OBJECT(ViewmodelAnimationController, LogicComponent);
public:
	ViewmodelAnimationController();
	~ViewmodelAnimationController();

	void Update(float dt) override;

private:
	int m_animationIndex = -1;
};

ViewmodelAnimationController::ViewmodelAnimationController()
{
}

ViewmodelAnimationController::~ViewmodelAnimationController()
{
}

void ViewmodelAnimationController::Update(float dt)
{
}

// More beautiful way to register classes
void registerGameClasses()
{
	const TypeInfo* gameClasses[] = 
	{
		OBJECT_GET_TYPEINFO(MainMenuWorldComponent),
		OBJECT_GET_TYPEINFO(PlayerControllerComponent),
		OBJECT_GET_TYPEINFO(WeaponComponent),
		OBJECT_GET_TYPEINFO(WeaponAutoComponent),
		OBJECT_GET_TYPEINFO(WeaponPistolComponent),
		OBJECT_GET_TYPEINFO(WeaponChainComponent),
		OBJECT_GET_TYPEINFO(LevelManagerComponent),
		OBJECT_GET_TYPEINFO(TestRotatorComponent),
		OBJECT_GET_TYPEINFO(TestPositionUpdaterComponent),
		OBJECT_GET_TYPEINFO(TestAnimationComponent),
		OBJECT_GET_TYPEINFO(ViewmodelAnimationController),
	};

	for (int i = 0; i < sizeof(gameClasses) / sizeof(gameClasses[0]); i++)
		TypeManager::GetInstance()->RegisterType(gameClasses[i]);
}

void registerShockClasses()
{
	const TypeInfo* shockClasses[] =
	{
		OBJECT_GET_TYPEINFO(ShockSignal),
		OBJECT_GET_TYPEINFO(ShockPlayerController),
		OBJECT_GET_TYPEINFO(ShockAIComponent),
		OBJECT_GET_TYPEINFO(ShockProjectileComponent),
		OBJECT_GET_TYPEINFO(ShockGameMainMenuComponent),
	};

	for (int i = 0; i < sizeof(shockClasses) / sizeof(shockClasses[0]); i++)
		TypeManager::GetInstance()->RegisterType(shockClasses[i]);
}

#define DEMO_GAME

ShockGameInterface::ShockGameInterface()
{
}

ShockGameInterface::~ShockGameInterface()
{
}

void ShockGameInterface::Initialize()
{
	Core::Msg("Initializing game");

	// register game objects
	registerGameClasses();

	// register shock objects
	registerShockClasses();

#ifdef DEMO_GAME
	TypeManager::GetInstance()->RegisterType(DemoGameMainMenuComponent::GetStaticTypeInfo());
#endif // DEMO_GAME

	// add event listener
//	g_eventManager.addEventListener(&g_shockEventListener);

	// Create signal manager
	ShockSignalManager::CreateInstance();
}

void ShockGameInterface::Shutdown()
{
	// Destroy signal manager
	ShockSignalManager::DestroyInstance();

	// g_eventManager.removeEventListener(&g_shockEventListener);
}

}