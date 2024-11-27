#ifndef SHOCK_AI_BT_NODE_SEARCH_TARGET_H
#define SHOCK_AI_BT_NODE_SEARCH_TARGET_H

#include "engine/ai/behaviour_tree.h"

namespace solunar
{
	class BehaviourTreeActionNodeZombieSearchTarget : public BehaviourTreeNode
	{
	public:
		BehaviourTreeActionNodeZombieSearchTarget(const char* pDebugName);
		~BehaviourTreeActionNodeZombieSearchTarget();

		eBehaviourTreeStatus Update(float dt) override;
	};
}

#endif