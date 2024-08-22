#include "enginepch.h"
#include "engine/physics/triggercomponent.h"
#include "engine/physics/rigidbodycomponent.h"

namespace solunar
{

TriggerComponent::TriggerComponent()
{
	m_isOnce = false;
}

TriggerComponent::~TriggerComponent()
{
}

void TriggerComponent::RegisterObject()
{
	TypeManager::GetInstance()->RegisterObject<TriggerComponent>();
}

void TriggerComponent::LoadXML(tinyxml2::XMLElement& element)
{

}

void TriggerComponent::SaveXML(tinyxml2::XMLElement& element)
{
}

void TriggerComponent::OnCollide(RigidBodyComponent* selfBody, RigidBodyComponent* touchingBody)
{
	// #TODO: !!!

	Core::Msg("(TriggerComponent::OnCollide): Trigger 0x%p, Enter 0x%p object", 
		selfBody->GetEntity(),
		touchingBody->GetEntity());
}

}