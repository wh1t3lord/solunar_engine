#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

namespace engine {
	
	class PlayerControllerComponent : public LogicComponent
	{
		ImplementObject(PlayerControllerComponent, LogicComponent);
	public:
		static void registerObject();
		
	public:
		PlayerControllerComponent();
		~PlayerControllerComponent();
		
		virtual void loadXML(tinyxml2::XMLElement& element);
		virtual void saveXML(tinyxml2::XMLElement& element);

		virtual void onEntitySet(Entity* entity);
		
		virtual void update(float dt);
		
	protected:
	//	std::shared_ptr<Node> m_cameraNode;
	
		bool m_onTheGround;
	};
	
}

#endif