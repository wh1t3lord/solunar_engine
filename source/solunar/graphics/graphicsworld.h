#ifndef GRAPHICSWORLD_H
#define GRAPHICSWORLD_H

#include "graphics/lightmanager.h"

namespace solunar {
	
	class DrawableComponent;
	
	class GraphicsWorld : public Object
	{
		IMPLEMENT_OBJECT(GraphicsWorld, Object);
	public:
		static void RegisterObject();
		
	public:
		GraphicsWorld();
		~GraphicsWorld();

		void OnWorldSet(World* world);

		void addDrawable(DrawableComponent* drawable);
		void removeDrawable(DrawableComponent* drawable);

		LightManager* GetLightManager() { return &m_lightManager; }

		const std::vector<DrawableComponent*>& getDrawables() { return m_drawables; }

		void updateDrawables();
		
	private:
		std::vector<DrawableComponent*> m_drawables;

		LightManager m_lightManager;
	};
	
}

#endif