#include "gamepch.h"
#include "game/gamelogic/player/playercontrollercomponent.h"

namespace solunar {
	
	void PlayerControllerComponent::registerObject()
	{
		g_typeManager->registerObject<PlayerControllerComponent>();
	}
	
	PlayerControllerComponent::PlayerControllerComponent()
	{
		m_onTheGround = false;
	}
	
	PlayerControllerComponent::~PlayerControllerComponent()
	{
		
	}

	void PlayerControllerComponent::loadXML(tinyxml2::XMLElement& element)
	{
		SerializableObject::loadXML(element);

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

	void PlayerControllerComponent::saveXML(tinyxml2::XMLElement& element)
	{
		SerializableObject::saveXML(element);
	}
	
	void PlayerControllerComponent::onEntitySet(Entity* entity)
	{
		LogicComponent::onEntitySet(entity);
	}
	
	void PlayerControllerComponent::update(float dt)
	{
		LogicComponent::update(dt);
	}
}