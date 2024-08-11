#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "engine/entity/logiccomponent.h"

namespace solunar {
	
	class PlayerControllerComponent : public LogicComponent
	{
		IMPLEMENT_OBJECT(PlayerControllerComponent, LogicComponent);
	public:
		static void RegisterObject();
		
	public:
		PlayerControllerComponent();
		~PlayerControllerComponent();
		
		virtual void LoadXML(tinyxml2::XMLElement& element);
		virtual void SaveXML(tinyxml2::XMLElement& element);

		virtual void OnEntitySet(Entity* entity);
		
		virtual void Update(float dt);
		
	protected:
	//	std::shared_ptr<Node> m_cameraNode;
	
		bool m_onTheGround;
	};
	
}

#endif