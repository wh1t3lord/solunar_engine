#ifndef ENGINE_H
#define ENGINE_H

namespace solunar
{
	class World;
	class IGameInterface;

	class Engine
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

	class EngineStateManager : public Singleton<EngineStateManager>
	{
	public:
		EngineStateManager();
		~EngineStateManager();

		void loadWorld(const std::string& filename);
		void loadEmptyWorld();
	
		void update();

	private:
		void onStateSwitch();

	private:
		std::string m_worldName;
		EngineState m_currentState;
		EngineState m_nextState;
	};

	struct EngineData
	{
		bool m_shouldCaptureMouse = false;
		bool m_shouldHideMouse = false;
	};

	extern EngineData g_engineData;
}

#endif // !ENGINE_H
