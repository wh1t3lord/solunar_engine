#include "shock_component_ai_behaviourtree.h"
#include "engine/ai/behaviour_tree.h"

namespace solunar
{
	IMPLEMENT_OBJECT(ShockAIBehaviourTree, LogicComponent);

	ShockAIBehaviourTree::ShockAIBehaviourTree()
	{
	}

	ShockAIBehaviourTree::~ShockAIBehaviourTree()
	{
	}

	void ShockAIBehaviourTree::Update(float dt)
	{
		if (m_pTree)
		{
			m_pTree->Update(dt);
		}
	}

	void ShockAIBehaviourTree::LoadXML(tinyxml2::XMLElement& element)
	{
		World* pWorld = this->GetWorld();
	}

	void ShockAIBehaviourTree::SaveXML(tinyxml2::XMLElement& element)
	{
	}
}