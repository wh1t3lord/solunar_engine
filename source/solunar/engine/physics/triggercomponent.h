#ifndef ENGINE_PHYSICS_TRIGGERCOMPONENT_H
#define ENGINE_PHYSICS_TRIGGERCOMPONENT_H

#include "engine/entity/logiccomponent.h"

namespace solunar
{

enum TriggerFilter
{
	TriggerFilter_Everything,
	TriggerFilter_Player,

	// RESERVED FIELDS

	// 256 is maximum
	TriggerFilter_Count = 256, 
};

class RigidBodyComponent;

class TriggerComponent : public Component
{
	IMPLEMENT_OBJECT(TriggerComponent, Component);
	DECLARE_PROPERTY_REGISTER(TriggerComponent);
public:
	TriggerComponent();
	~TriggerComponent();

	static void RegisterObject();

	// SerializableObject
	void LoadXML(tinyxml2::XMLElement& element) override;
	void SaveXML(tinyxml2::XMLElement& element) override;

	// Trigger

	void OnCollide(RigidBodyComponent* selfBody, RigidBodyComponent* touchingBody);

private:
	bool m_isOnce;
};

}

#endif // !ENGINE_PHYSICS_TRIGGERCOMPONENT_H
