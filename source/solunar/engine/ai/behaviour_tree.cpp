#include "behaviour_tree.h"
#include "tinyxml2.h"

namespace solunar
{
    BehaviourTreeNode::BehaviourTreeNode(const char* pDebugName) : m_update(true), m_id(kBehaviourTreeInvalidBaseType), m_priority(kBehaviourTreeInvalidBaseType), m_children_count(0)
    {
#ifdef _DEBUG
        constexpr size_t _kDebugStringMaxLength = sizeof(m_debug_name) / sizeof(m_debug_name[0]);
        std::memcpy(m_debug_name, pDebugName, min(strlen(pDebugName), _kDebugStringMaxLength));
#endif
    }

    BehaviourTreeNode::~BehaviourTreeNode()
    {
    }

    eBehaviourTreeStatus BehaviourTreeNode::Update(World* pWorld, void* pUserStateData, BehaviourTreeNode** pChildren, unsigned char children_count, float dt)
    {
        return eBehaviourTreeStatus::kFailure;
    }

    void BehaviourTreeNode::OnEvent(int event_id)
    {
    }

    const char* BehaviourTreeNode::GetName(void) const
    {
#ifdef _DEBUG
        return m_debug_name;
#else
        return nullptr;
#endif
    }

    unsigned char BehaviourTreeNode::GetChildrenCount(void) const
    {
        return m_children_count;
    }

    void BehaviourTreeNode::SetChildrenCount(unsigned char count)
    {
        m_children_count = count;
    }

    void BehaviourTreeNode::SetID(unsigned char id)
    {
        m_id = id;
    }

    unsigned char BehaviourTreeNode::GetID(void) const
    {
        return m_id;
    }

    void BehaviourTreeNode::SetPriority(unsigned char priority)
    {
        m_priority = priority;
    }

    unsigned char BehaviourTreeNode::GetPriority(void) const
    {
        return m_priority;
    }

    bool BehaviourTreeNode::CanUpdate() const
    {
        return m_update;
    }

    void BehaviourTreeNode::SetCanUpdate(bool status)
    {
        m_update = status;
    }
}