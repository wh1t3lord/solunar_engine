#ifndef ENGINE_WORLDMANAGER_H
#define ENGINE_WORLDMANAGER_H

namespace solunar
{

class World;
class WorldManagerImpl;

class WorldManager
{
public:
	static void Init();
	static void Shutdown();

	static void SetActiveWorld(World* world);
	static World* GetActiveWorld();

	static void ReleaseWorld();

private:
	static WorldManagerImpl* ms_impl;
};

}

#endif // !ENGINE_WORLDMANAGER_H
