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
	ImplementObject(TestRotatorComponent, LogicComponent);

public:
	TestRotatorComponent() : m_YAxis(0.0f)
	{}

	~TestRotatorComponent()
	{}

	static void registerObject()
	{
		g_typeManager->registerObject<TestRotatorComponent>();
	}

	void update(float dt) override
	{
		m_YAxis += dt * 200.0f;
		getEntity()->quaternionRotate(glm::vec3(0.f, 1.f, 0.f), m_YAxis);
	}

private:
	float m_YAxis;
};

class TestPositionUpdaterComponent : public LogicComponent
{
	ImplementObject(TestPositionUpdaterComponent, LogicComponent);

public:
	TestPositionUpdaterComponent()
	{}

	~TestPositionUpdaterComponent()
	{}

	static void registerObject()
	{
		g_typeManager->registerObject<TestPositionUpdaterComponent>();
	}

	void update(float dt) override
	{
		const float kSpeed = 2.0f;

		glm::vec3 position = getEntity()->getPosition();
		position.y += kSpeed * dt;
		getEntity()->setPosition(position);
	}

};

class TestAnimationComponent : public LogicComponent
{
	ImplementObject(TestAnimationComponent, LogicComponent);
public:
	TestAnimationComponent();
	~TestAnimationComponent();

	void loadXML(tinyxml2::XMLElement& element) override;

	void update(float dt) override;

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

void TestAnimationComponent::loadXML(tinyxml2::XMLElement& element)
{
	tinyxml2::XMLElement* animationNameElement = element.FirstChildElement("AnimationName");
	if (animationNameElement)
	{
		const tinyxml2::XMLAttribute* animationNameAttribute = animationNameElement->FindAttribute("value");
		if (animationNameAttribute)
		{
			m_animationName = animationNameAttribute->Value();
		}
	}
}

void TestAnimationComponent::update(float dt)
{
	AnimatedMeshComponent* animatedMeshComponent = getEntity()->getComponent<AnimatedMeshComponent>();
	std::shared_ptr<ModelBase> modelBase = animatedMeshComponent->lockModel();
	AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(modelBase.get());
	if (animatedModel) {
		if (m_animationIndex == -1) {
			m_animationIndex = animatedModel->getAnimationByName(m_animationName);
			animatedModel->setPlayAnimation(m_animationIndex, true);
		}
	}

	animatedModel->testPlay(dt);
}

class ViewmodelAnimationController : public LogicComponent
{
	ImplementObject(ViewmodelAnimationController, LogicComponent);
public:
	ViewmodelAnimationController();
	~ViewmodelAnimationController();

	void update(float dt) override;

private:
	int m_animationIndex = -1;
};

ViewmodelAnimationController::ViewmodelAnimationController()
{
}

ViewmodelAnimationController::~ViewmodelAnimationController()
{
}

void ViewmodelAnimationController::update(float dt)
{
	AnimatedMeshComponent* animatedMeshComponent = getEntity()->getComponent<AnimatedMeshComponent>();
	std::shared_ptr<ModelBase> modelBase = animatedMeshComponent->lockModel();
	AnimatedModel* animatedModel = dynamicCast<AnimatedModel>(modelBase.get());
	if (animatedModel) {
		if (m_animationIndex == -1) {
			m_animationIndex = animatedModel->getAnimationByName("Armature|Armature|Hide");
			animatedModel->setPlayAnimation(m_animationIndex, true);
			animatedModel->pauseAnimationPlay();
		}
	}

	animatedModel->testPlay(dt);
}

// More beautiful way to register classes
void registerGameClasses()
{
	const TypeInfo* gameClasses[] = 
	{
		ObjectGetTypeInfo(MainMenuWorldComponent),
		ObjectGetTypeInfo(PlayerControllerComponent),
		ObjectGetTypeInfo(WeaponComponent),
		ObjectGetTypeInfo(WeaponAutoComponent),
		ObjectGetTypeInfo(WeaponPistolComponent),
		ObjectGetTypeInfo(WeaponChainComponent),
		ObjectGetTypeInfo(LevelManagerComponent),
		ObjectGetTypeInfo(TestRotatorComponent),
		ObjectGetTypeInfo(TestPositionUpdaterComponent),
		ObjectGetTypeInfo(TestAnimationComponent),
		ObjectGetTypeInfo(ViewmodelAnimationController),
	};

	for (int i = 0; i < sizeof(gameClasses) / sizeof(gameClasses[0]); i++)
		TypeManager::getInstance()->registerType(gameClasses[i]);
}

void registerShockClasses()
{
	const TypeInfo* shockClasses[] =
	{
		ObjectGetTypeInfo(ShockSignal),
		ObjectGetTypeInfo(ShockPlayerController),
		ObjectGetTypeInfo(ShockAIComponent),
		ObjectGetTypeInfo(ShockProjectileComponent),
		ObjectGetTypeInfo(ShockGameMainMenuComponent),
	};

	for (int i = 0; i < sizeof(shockClasses) / sizeof(shockClasses[0]); i++)
		TypeManager::getInstance()->registerType(shockClasses[i]);
}

#define DEMO_GAME

ShockGameInterface::ShockGameInterface()
{
}

ShockGameInterface::~ShockGameInterface()
{
}

void ShockGameInterface::initialize()
{
	Core::msg("Initializing game");

	// register game objects
	registerGameClasses();

	// register shock objects
	registerShockClasses();

#ifdef DEMO_GAME
	TypeManager::getInstance()->registerType(DemoGameMainMenuComponent::getStaticTypeInfo());
#endif // DEMO_GAME

	// add event listener
//	g_eventManager.addEventListener(&g_shockEventListener);

	// Create signal manager
	ShockSignalManager::createInstance();
}

void ShockGameInterface::shutdown()
{
	// Destroy signal manager
	ShockSignalManager::destroyInstance();

	// g_eventManager.removeEventListener(&g_shockEventListener);
}

}