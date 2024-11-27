#include "shock_ai_behaviour_tree_factory.h"
#include "engine/ai/behaviour_tree.h"

#include "shock_ai_behaviour_tree_node_sequence.h"


#include "shock_ai_behaviour_tree_node_zombie_search_target.h"
#include "shock_ai_behaviour_tree_node_zombie_move_to_target.h"
#include "shock_ai_behaviour_tree_node_zombie_attack_target.h"

namespace solunar
{
	IBehaviourTree* CreateBehaviourTree(eShockBehaviourTree type)
	{
		switch (type)
		{
		case eShockBehaviourTree::kZombie:
		{
			auto* pInstance = new BehaviourTree<BehaviourTreeLinearAllocator<5, 128>, 3>("zombie_tree");

			pInstance->Init();
			
			const unsigned char _kMaxChildrenCountMainSequence = 2;
			const unsigned char _kMaxChildrenCountSubSequence = 2;
			auto* pSequence = pInstance->AddNode<BehaviourTreeNodeSequence<_kMaxChildrenCountMainSequence>>("zombie_seq_main");
			pInstance->AddNode<BehaviourTreeActionNodeZombieSearchTarget>(pSequence->GetID(), "zombie_act_search_target");
			
			auto* pSequenceSub = pInstance->AddNode<BehaviourTreeNodeSequence<_kMaxChildrenCountSubSequence>>(pSequence->GetID(), "zombie_seq_move_and_attack");
			pInstance->AddNode<BehaviourTreeActionNodeZombieMoveToTarget>(pSequenceSub->GetID(), "zombie_act_move_to_target");
			pInstance->AddNode<BehaviourTreeActionNodeZombieAttackTarget>(pSequenceSub->GetID(), "zombie_act_attack_target");

			return pInstance;
		}
		default:
		{
			Assert(false && "not implemented type of behaviour tree. Game doesn't support it!");
			return nullptr;
		}
		}
	}
}
 