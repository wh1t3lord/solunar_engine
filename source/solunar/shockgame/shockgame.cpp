#include "shockgamepch.h"
#include "engine/gameinterface.h"
#include "shockgame/shockgame.h"
#include "shockgame/shockeventlistener.h"
#include "shockgame/shocksignalmanager.h"
#include "shockgame/shockplayercontroller.h"

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

#include "graphics/imguimanager.h"

namespace engine
{

// implemetation for IGameInterface
static ShockGameInterface s_shockGameInterface;

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

void registerGameClasses()
{
	MainMenuWorldComponent::registerObject();
	PlayerControllerComponent::registerObject();
	WeaponComponent::registerObject();
	WeaponAutoComponent::registerObject();
	WeaponPistolComponent::registerObject();
	WeaponChainComponent::registerObject();
	LevelManagerComponent::registerObject();

	TestRotatorComponent::registerObject();
}

void registerShockClasses()
{
	ShockSignal::registerObject();
	 
	ShockPlayerController::registerObject();
}

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

void shockGamePlayerDebug(bool* open)
{
	if (ImGui::Begin("Shock Player Debug", open))
	{
		World* world = Engine::ms_world;
		if (world)
		{
			std::vector<Entity*> players = world->getEntityManager().getEntitiesWithComponent<ShockPlayerController>();
			if (!players.empty())
			{
				Entity* player = players[0];
				ShockPlayerController* playerController = player->getComponent<ShockPlayerController>();
				if (playerController)
				{
					ImGui::Checkbox("Fly cam", &playerController->m_flyCam);
				}
			}
			else
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Player with ShockPlayerController doesn't exist!");
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "World is not loaded!");
		}
	}

	ImGui::End();

}

}

#ifdef WIN32

using namespace engine;

__declspec(dllexport) void dummy_export()
{

}

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// #TODO: !!!
		::g_gameInterface = &s_shockGameInterface;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif // WIN32
