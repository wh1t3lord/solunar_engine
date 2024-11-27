#ifndef BEHAVIOUR_TREE_H
#define BEHAVIOUR_TREE_H

namespace tinyxml2
{
	class XMLElement;
}

namespace solunar
{
	enum eBehaviourTreeStatus
	{
		kFailure,
		kRunning,
		kSuccess
	};

	// user passes look-up table with timings for each behaviour tree, it has "default array" if user didn't specify anything
	enum eBehaviourTreeNodePriority
	{
		// rarely updates
		kLow,
		// not often updates
		kMedium,
		// always updates
		kHigh,
		kSize
	};

	const unsigned char kBehaviourTreeMaxCountOfChildren = 8;

	class BehaviourTreeNode
	{
	public:
		BehaviourTreeNode();
		virtual ~BehaviourTreeNode();

		virtual void AddNode(BehaviourTreeNode* pAllocateNode);
		virtual eBehaviourTreeStatus Update(float dt);
		virtual const char* GetName(void) const;

		void SetID(unsigned char id);
		unsigned char GetID(void) const;

		void SetPriority(unsigned char priority);
		unsigned char GetPriority(void) const;

		bool CanUpdate() const;
		void SetCanUpdate(bool status);
	private:
		// in case of linear allocator we can't delete node
		bool m_update;
		// allocated id from allocator
		unsigned char m_id;
		unsigned char m_priority;
	};

	class IBehaviourTreeAllocator
	{
	public:
		virtual ~IBehaviourTreeAllocator() {}

		virtual void* AllocateNode(size_t size_of_class) = 0;
		virtual void DeleteNode(void* pNode) = 0;
	};

	class IBehaviourTree
	{
	public:
		virtual ~IBehaviourTree() {}
	};

	// choose size per user behaviour tree implementation, it might be very different!
	// todo: wh1t3lord -> provide implementation of allocator for case where node deletes and user can make behaviour tree dynamic like in runtime adds new nodes and etc
	template<unsigned char MaxNumberOfNodes, size_t MaxSizeOfBehaviourTreeNodeInBytes>
	class BehaviourTreeLinearAllocator : public IBehaviourTreeAllocator
	{
	public:
		BehaviourTreeLinearAllocator() :IBehaviourTreeAllocator(), m_allocated_nodes_count(0) {}
		~BehaviourTreeLinearAllocator() {}

		// use this for placement new
		// e.g.: new (pOffsetFromAllocateNodePointer) BehaviourTreeNodeXXX()
		void* AllocateNode(size_t size_of_class) override
		{
			Assert(m_allocated_nodes_count < MaxNumberOfNodes && "overflow you need to shrink your allocator");
			Assert(size_of_class <= MaxSizeOfBehaviourTreeNodeInBytes && "can't allocate memory for a such data! You specified not the highest class that might use in your tree");

			void* pResult = nullptr;

			if (size_of_class <= MaxSizeOfBehaviourTreeNodeInBytes)
			{
				pResult = &m_memory_buffer[m_allocated_nodes_count * MaxSizeOfBehaviourTreeNodeInBytes];
			}

			++m_allocated_nodes_count;

			return pResult;
		}

		void DeleteNode(void* pNode) override
		{
			// no sense for linear
			// but we need to keep arch same

			BehaviourTreeNode* pNode = static_cast<BehaviourTreeNode*>(pNode);

			if (pNode)
			{
				pNode->SetCanUpdate(false);
			}
		}

		constexpr static unsigned char GetMaxNodes() {
			return MaxNumberOfNodes;
		}

	private:
		unsigned char m_allocated_nodes_count;
		unsigned char m_memory_buffer[MaxNumberOfNodes * MaxSizeOfBehaviourTreeNodeInBytes];
	};

	// low priority - updates after 1 second; medium priority - updates after half of second; high priority - updates immedeatily without pauses;
	const float kBehaviourTreeDefaultUserPriorityTimings[eBehaviourTreeNodePriority::kSize] = { 1.0f, 0.5f, 0.0f };

	/*
		How to work with this?

		You should specify allocator, for now there's only one allocator is linear, for future you can develop an allocator for general-purpose use and make behaviour tree fully dynamic and change nodes at runtime (it might be useful)

		For making ultra fast performant implementation allocator provides new placement policy of allocating nodes. Because we can know at compile time how many nodes will BT use and how REAL memory needs to handle a such tree. It provides best optimization between memory consumption/performance.

		Just add forever node and you get a node that is always updates (every frame)

		Then add some node that you want:

			1. Selector node
				Updates until some of nodes return success

			2. Sequence node
				Updates until some of nodes return failure

			3. Forever node
				Always updates no matter of status

			4. Once
				Once updates then removes from tree

			5.

		Node that stopped updates it means that a node was deleted from a tree, depends on allocator
	*/
	template<typename Allocator, unsigned char MaxNodesInTree>
	class BehaviourTree : public IBehaviourTree
	{
		static_assert(Allocator::GetMaxNodes() >= MaxNodesInTree && "suppose that allocator can allocate at least MaxNodeInTree! Sadly but in C++ we can't obtain template parameter from class :(");
	public:
		BehaviourTree(const char* pDebugName);
		~BehaviourTree();

		void Init(float p_user_priority_timings[eBehaviourTreeNodePriority::kSize] = 0);
		void Update(float dt);
		void Shutdown();

		const char* GetName(void) const;

		void GetAllNodes(BehaviourTreeNode**& pNodes, unsigned char& count);

	private:
		unsigned char m_nodes_count;
#ifdef _DEBUG
		char m_debug_name[16];
#endif
		BehaviourTreeNode* m_pNodes[MaxNodesInTree];
		Allocator m_allocator;
	};

	template<typename Allocator, unsigned char MaxNodesInTree>
	inline BehaviourTree<Allocator, MaxNodesInTree>::BehaviourTree(const char* pDebugName)
	{
	}
	template<typename Allocator, unsigned char MaxNodesInTree>
	inline BehaviourTree<Allocator, MaxNodesInTree>::~BehaviourTree()
	{
	}
	template<typename Allocator, unsigned char MaxNodesInTree>
	inline void BehaviourTree<Allocator, MaxNodesInTree>::Init(float p_user_priority_timings[eBehaviourTreeNodePriority::kSize])
	{
	}
	template<typename Allocator, unsigned char MaxNodesInTree>
	inline void BehaviourTree<Allocator, MaxNodesInTree>::Update(float dt)
	{
	}
	template<typename Allocator, unsigned char MaxNodesInTree>
	inline void BehaviourTree<Allocator, MaxNodesInTree>::Shutdown()
	{
	}
	template<typename Allocator, unsigned char MaxNodesInTree>
	inline const char* BehaviourTree<Allocator, MaxNodesInTree>::GetName(void) const
	{
#ifdef _DEBUG
		return m_debug_name;
#else
		return nullptr;
#endif
	}
	template<typename Allocator, unsigned char MaxNodesInTree>
	inline void BehaviourTree<Allocator, MaxNodesInTree>::GetAllNodes(BehaviourTreeNode**& pNodes, unsigned char& count)
	{
		pNodes = this->m_pNodes;
		count = this->m_nodes_count;
	}
}

#endif // !BEHAVIOUR_TREE_H
