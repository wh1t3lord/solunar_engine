#ifndef LOGICCOMPONENT_H
#define LOGICCOMPONENT_H

#include "engine/entity/component.h"

namespace solunar
{
	// Logic component for node (run update function once at game frame)
	class LogicComponent : public Component
	{
	public:
		ImplementObject(LogicComponent, Component);

	public:
		LogicComponent();
		virtual ~LogicComponent();

		// Update component once per frame.
		virtual void update(float delta);
	};
}

#endif // !LOGICCOMPONENT_H
