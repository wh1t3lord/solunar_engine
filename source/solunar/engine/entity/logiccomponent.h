#ifndef LOGICCOMPONENT_H
#define LOGICCOMPONENT_H

#include "engine/entity/component.h"

namespace solunar
{
	// Logic component for node (run update function once at game frame)
	class LogicComponent : public Component
	{
	public:
		IMPLEMENT_OBJECT(LogicComponent, Component);

	public:
		LogicComponent();
		virtual ~LogicComponent();

		// Update component once per frame.
		virtual void Update(float delta);
	};
}

#endif // !LOGICCOMPONENT_H
