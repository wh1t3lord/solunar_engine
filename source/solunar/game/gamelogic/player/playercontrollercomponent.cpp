#include "gamepch.h"
#include "game/gamelogic/player/playercontrollercomponent.h"

namespace solunar {
	
	IMPLEMENT_OBJECT(PlayerControllerComponent, LogicComponent);

	void PlayerControllerComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<PlayerControllerComponent>();
	}
	
	PlayerControllerComponent::PlayerControllerComponent()
	{
		m_onTheGround = false;
	}
	
	PlayerControllerComponent::~PlayerControllerComponent()
	{
		
	}

	void PlayerControllerComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		SerializableObject::LoadXML(element);

		tinyxml2::XMLElement* onGroundProperty = element.FirstChildElement("OnGround");
		if (onGroundProperty)
		{
			const tinyxml2::XMLAttribute* onGroundPropertyValue = onGroundProperty->FindAttribute("value");
			if (onGroundPropertyValue)
			{
				onGroundPropertyValue->QueryBoolValue(&m_onTheGround);
			}
		}
	}

	void PlayerControllerComponent::SaveXML(tinyxml2::XMLElement& element)
	{
		SerializableObject::SaveXML(element);
	}
	
	void PlayerControllerComponent::OnEntitySet(Entity* entity)
	{
		LogicComponent::OnEntitySet(entity);
	}
	
	void PlayerControllerComponent::Update(float dt)
	{
		LogicComponent::Update(dt);
	}
}