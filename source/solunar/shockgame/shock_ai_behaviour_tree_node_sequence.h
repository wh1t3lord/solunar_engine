#ifndef SHOCK_AI_BT_NODE_SEQUENCE_H
#define SHOCK_AI_BT_NODE_SEQUENCE_H

#include "engine/ai/behaviour_tree.h"

namespace solunar
{
	template<unsigned char MaxChildrenCount>
	class BehaviourTreeNodeSequence : public BehaviourTreeNode
	{
	public:
		BehaviourTreeNodeSequence(const char* pDebugName);
		~BehaviourTreeNodeSequence();

		eBehaviourTreeStatus Update(float dt) override;
		void OnEvent(int event_id) override;


		// only parent defines children
		BehaviourTreeNode** GetChildren(void) override;
		unsigned char GetChildrenMaxCount(void) const override;
		unsigned char GetChildrenCount(void) const override;

		void SetChild(unsigned char array_index, BehaviourTreeNode* pChild) override;

	private:
		unsigned char m_current_nodes_count;
		unsigned char m_success_iter;
		BehaviourTreeNode* m_pChildren[MaxChildrenCount];
	};

	template<unsigned char MaxChildrenCount>
	inline BehaviourTreeNodeSequence<MaxChildrenCount>::BehaviourTreeNodeSequence(const char* pDebugName) : BehaviourTreeNode(pDebugName), m_current_nodes_count(0), m_success_iter(0), m_pChildren{}
	{
	}

	template<unsigned char MaxChildrenCount>
	inline BehaviourTreeNodeSequence<MaxChildrenCount>::~BehaviourTreeNodeSequence()
	{
	}

	template<unsigned char MaxChildrenCount>
	inline eBehaviourTreeStatus BehaviourTreeNodeSequence<MaxChildrenCount>::Update(float dt)
	{
		eBehaviourTreeStatus status = eBehaviourTreeStatus::kSuccess;

		if (m_success_iter == MaxChildrenCount)
		{
			m_success_iter = 0;
			return status;
		}
		
		BehaviourTreeNode* pChild = m_pChildren[m_success_iter];

		Assert(pChild && "can't be!");

		if (pChild)
		{
			status = pChild->Update(dt);
		}

		if (status == eBehaviourTreeStatus::kSuccess)
			status = eBehaviourTreeStatus::kRunning;

		++m_success_iter;
		return status;
	}

	template<unsigned char MaxChildrenCount>
	inline void BehaviourTreeNodeSequence<MaxChildrenCount>::OnEvent(int event_id)
	{
		// for future
	}

	template<unsigned char MaxChildrenCount>
	inline BehaviourTreeNode** BehaviourTreeNodeSequence<MaxChildrenCount>::GetChildren(void)
	{
		return m_pChildren;
	}

	template<unsigned char MaxChildrenCount>
	inline unsigned char BehaviourTreeNodeSequence<MaxChildrenCount>::GetChildrenMaxCount(void) const
	{
		return MaxChildrenCount;
	}

	template<unsigned char MaxChildrenCount>
	inline unsigned char BehaviourTreeNodeSequence<MaxChildrenCount>::GetChildrenCount(void) const
	{
		return m_current_nodes_count;
	}

	template<unsigned char MaxChildrenCount>
	inline void BehaviourTreeNodeSequence<MaxChildrenCount>::SetChild(unsigned char array_index, BehaviourTreeNode* pChild)
	{
		m_pChildren[array_index] = pChild;
		++m_current_nodes_count;
	}
}

#endif