// -----------------------------------------
//
//   author: wh1t3lord (https://github.com/wh1t3lord)
//	 description: Simple but effective Behaviour tree factory (creation function based on pre-defined type)
//
// -----------------------------------------


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