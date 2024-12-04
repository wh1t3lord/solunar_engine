#include "shock_ai_behaviour_tree_node_zombie_search_target.h"

namespace solunar
{
	BehaviourTreeActionNodeZombieSearchTarget::BehaviourTreeActionNodeZombieSearchTarget(const char* pDebugName): BehaviourTreeNode(pDebugName)
	{
	}

	BehaviourTreeActionNodeZombieSearchTarget::~BehaviourTreeActionNodeZombieSearchTarget()
	{
	}

	eBehaviourTreeStatus BehaviourTreeActionNodeZombieSearchTarget::Update(World* pWorld, void* pUserStateData, float dt)
	{
		return eBehaviourTreeStatus::kFailure;
	}
}