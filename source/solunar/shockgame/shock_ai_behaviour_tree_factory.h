#ifndef SHOCK_AI_BT_FACTORY_H
#define SHOCK_AI_BT_FACTORY_H

namespace solunar
{
	class IBehaviourTree;
	class World;
}

namespace solunar
{
	enum eShockBehaviourTree 
	{
		kZombie
	};

	IBehaviourTree* CreateBehaviourTree(World* pLoadedWorld, eShockBehaviourTree type);
}

#endif