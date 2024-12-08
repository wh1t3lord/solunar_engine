#include "enginepch.h"
#include "engine/entity/world.h"
#include "engine/worldmanager.h"

namespace solunar
{

WorldManagerImpl* WorldManager::ms_impl = nullptr;

/////////////////////////////////////////////////////////////////
// World manager impl
class WorldManagerImpl
{
public:
	void Init();
	void Shutdown();

	void SetActiveWorld(World* world);
	World* GetActiveWorld();

	void ReleaseWorld();

private:
	World* m_world;
};

void WorldManagerImpl::Init()
{
	m_world = nullptr;
}

void WorldManagerImpl::Shutdown()
{
	ReleaseWorld();
}

void WorldManagerImpl::SetActiveWorld(World* world)
{
	Assert(world);
	m_world = world;
}

World* WorldManagerImpl::GetActiveWorld()
{
	return m_world;
}

void WorldManagerImpl::ReleaseWorld()
{
	if (m_world)
	{
		mem_delete(m_world);
		m_world = nullptr;
	}
}

///////////////////////////////////////////////////////////////
// Interface

void WorldManager::Init()
{
	ms_impl = mem_new<WorldManagerImpl>();
	ms_impl->Init();
}

void WorldManager::Shutdown()
{
	if (ms_impl)
	{
		ms_impl->Shutdown();

		mem_delete(ms_impl);
		ms_impl = nullptr;
	}
}

void WorldManager::SetActiveWorld(World* world)
{
	return ms_impl->SetActiveWorld(world);
}

World* WorldManager::GetActiveWorld()
{
	return ms_impl->GetActiveWorld();
}

void WorldManager::ReleaseWorld()
{
	return ms_impl->ReleaseWorld();
}

}