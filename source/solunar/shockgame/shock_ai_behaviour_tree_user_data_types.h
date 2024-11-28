#ifndef SHOCK_AI_BT_USER_DATA_TYPES_H
#define SHOCK_AI_BT_USER_DATA_TYPES_H

namespace solunar
{
	class Entity;
}

namespace solunar
{
	// stores information for handling Zombie's AI
	struct ZombieLogicStateType
	{
		bool can_reach_target;
		bool has_free_point;
		// todo: kirrik -> replace to id for accessing entity
		Entity* pTarget;
	};
}

#endif