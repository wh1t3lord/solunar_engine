#ifndef ENGINE_H
#define ENGINE_H

namespace engine
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
}

#endif // !ENGINE_H
