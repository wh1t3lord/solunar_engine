#ifndef ENGINE_PHYSICS_TRIGGERCOMPONENT_H
#define ENGINE_PHYSICS_TRIGGERCOMPONENT_H

#include "engine/entity/logiccomponent.h"

namespace solunar
{

class RigidBodyComponent;

class TriggerComponent : public Component
{
	IMPLEMENT_OBJECT(TriggerComponent, Component);
public:
	TriggerComponent();
	~TriggerComponent();

	static void RegisterObject();

	void OnCollide(RigidBodyComponent* selfBody, RigidBodyComponent* touchingBody);
};

}

#endif // !ENGINE_PHYSICS_TRIGGERCOMPONENT_H
