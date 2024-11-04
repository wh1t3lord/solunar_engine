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
		static void Init();
		static void Shutdown();

		static void LoadWorld(const std::string& filename);

		static void LoadEmptyWorld();

		static void Update();

		static void SetGameInterface(IGameInterface* gameInterface);
		static IGameInterface* GetGameInterface();
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

		void LoadWorld(const std::string& filename);
		void LoadEmptyWorld();
	
		void Update();

	private:
		void OnStateSwitch();

	private:
		std::string m_worldName;
		EngineState m_currentState;
		EngineState m_nextState;
	};

	struct EngineData
	{
		bool m_shouldCaptureMouse = false;
		bool m_shouldHideMouse = false;
		bool m_editor = false;
	};

	extern EngineData g_engineData;
}

#endif // !ENGINE_H
