#ifndef SHOCK_AI_BT_FACTORY_H
#define SHOCK_AI_BT_FACTORY_H

namespace solunar
{
	class IBehaviourTree;
}

namespace solunar
{
	enum eShockBehaviourTree 
	{
		kZombie
	};

	IBehaviourTree* CreateBehaviourTree(eShockBehaviourTree type);
}

#endif