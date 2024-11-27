#ifndef SHOCK_AI_BT_NODE_ZOMBIE_MOVE_TO_TARGET_H
#define SHOCK_AI_BT_NODE_ZOMBIE_MOVE_TO_TARGET_H

#include "engine/ai/behaviour_tree.h"

namespace solunar
{
	class BehaviourTreeActionNodeZombieMoveToTarget : public BehaviourTreeNode
	{
	public:
		BehaviourTreeActionNodeZombieMoveToTarget(const char* pDebugName);
		~BehaviourTreeActionNodeZombieMoveToTarget();

		eBehaviourTreeStatus Update(float dt) override;

	private:
	};
}

#endif