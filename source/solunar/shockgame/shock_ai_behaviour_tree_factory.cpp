#include "shock_ai_behaviour_tree_factory.h"
#include "engine/ai/behaviour_tree.h"

#include "shock_ai_behaviour_tree_node_sequence.h"

#include "shock_ai_behaviour_tree_user_data_types.h"
#include "shock_ai_behaviour_tree_node_zombie_search_target.h"
#include "shock_ai_behaviour_tree_node_zombie_move_to_target.h"
#include "shock_ai_behaviour_tree_node_zombie_attack_target.h"

namespace solunar
{
	IBehaviourTree* CreateBehaviourTree(World* pLoadedWorld, eShockBehaviourTree type)
	{
		Assert(pLoadedWorld && "must be valid!");

		switch (type)
		{
		case eShockBehaviourTree::kZombie:
		{
			constexpr size_t _kAllocatorMaxClassNodeSize = 128;
			constexpr unsigned char _kAllocatorReservedNodeCount = 5;
			constexpr unsigned char _kBehaviourTreeMaxNodesCount = 5;

			auto* pInstance = new BehaviourTree<BehaviourTreeLinearAllocator<_kAllocatorReservedNodeCount, _kAllocatorMaxClassNodeSize>, ZombieLogicStateType, _kBehaviourTreeMaxNodesCount>("zombie_tree");

			pInstance->Init(pLoadedWorld);
			
			constexpr unsigned char _kMaxChildrenCountMainSequence = 2;
			constexpr unsigned char _kMaxChildrenCountSubSequence = 2;
			auto* pSequence = pInstance->AddNode<BehaviourTreeNodeSequence<_kMaxChildrenCountMainSequence>>("zombie_seq_main");
			pInstance->AddNode<BehaviourTreeActionNodeZombieSearchTarget>(pSequence->GetID(), "zombie_act_search_target");
			
			auto* pSequenceSub = pInstance->AddNode<BehaviourTreeNodeSequence<_kMaxChildrenCountSubSequence>>(pSequence->GetID(), "zombie_seq_move_+_attack");
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
	eShockBehaviourTree ConvertStringToShockBehaviourTree(const char* pRawString)
	{

		if (!pRawString)
			return eShockBehaviourTree::kUnknown;

		if (!strcmp(pRawString, "Zombie"))
			return eShockBehaviourTree::kZombie;

		return eShockBehaviourTree::kUnknown;
	}
	const char* ConvertShockBehaviourTreeToString(eShockBehaviourTree type)
	{
		switch (type)
		{
		case eShockBehaviourTree::kZombie:
		{
			return "Zombie";
		}
		default:
		{
			Assert(!"not implemeneted");
			return "UNKNOWN_BEHAVIOUR_TREE_TYPE";
		}
		}
	}
}
 