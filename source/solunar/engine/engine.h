#ifndef ENGINE_H
#define ENGINE_H

#include "engine/engineapi.h"

namespace engine
{
	class World;
	class IGameInterface;

	class ENGINE_API Engine
	{
	public:
		static World* ms_world;

	public:
		static void init();
		static void shutdown();

		static void loadWorld(const std::string& filename);

		static void loadEmptyWorld();

		static void update();

		static void setGameInterface(IGameInterface* gameInterface);
		static IGameInterface* getGameInterface();
	};

	enum class EngineState : uint16_t
	{
		None,
		Running,
		LoadWorld,

		Count
	};

	class ENGINE_API EngineStateManager : public Singleton<EngineStateManager>
	{
	public:
		EngineStateManager();
		~EngineStateManager();

		void loadWorld(const std::string& filename);
		void loadEmptyWorld();
	
		void update();
	private:
		std::string m_worldName;
		EngineState m_currentState;
		EngineState m_nextState;
	};

	class IImGuiManager;
	class IRenderer;

	class ENGINE_API GraphicsFacade
	{
	public:
		static void init(void* window);
		static void shutdown();
		static bool loadSettings(const std::string& filename);
		static void saveSettings(const std::string& filename);
		static void applyDefaultOptions();
		static int getWidth();
		static int getHeight();
		static bool getFullscreen();
		static IImGuiManager* getImGuiManager();
		static IRenderer* getRenderer();
	};
}

#endif // !ENGINE_H
