#ifndef GRAPHICSWORLD_H
#define GRAPHICSWORLD_H

#include "graphics/lightmanager.h"

namespace solunar {
	
	class DrawableComponent;
	
	class GraphicsWorld : public Object
	{
		ImplementObject(GraphicsWorld, Object);
	public:
		static void registerObject();
		
	public:
		GraphicsWorld();
		~GraphicsWorld();

		void onWorldSet(World* world);

		void addDrawable(DrawableComponent* drawable);
		void removeDrawable(DrawableComponent* drawable);

		LightManager* getLightManager() { return &m_lightManager; }

		const std::vector<DrawableComponent*>& getDrawables() { return m_drawables; }

		void updateDrawables();
		
	private:
		std::vector<DrawableComponent*> m_drawables;

		LightManager m_lightManager;
	};
	
}

#endif