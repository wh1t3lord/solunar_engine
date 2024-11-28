#ifndef SHOCK_AI_BT_NODE_ZOMBIE_ATTACK_TARGET_H
#define SHOCK_AI_BT_NODE_ZOMBIE_ATTACK_TARGET_H

#include "engine/ai/behaviour_tree.h"

namespace solunar
{
	class BehaviourTreeActionNodeZombieAttackTarget : public BehaviourTreeNode
	{
	public:
		BehaviourTreeActionNodeZombieAttackTarget(const char* pDebugName);
		~BehaviourTreeActionNodeZombieAttackTarget();

		eBehaviourTreeStatus Update(World* pWorld, void* pUserStateData, float dt) override;
	};
}

#endif