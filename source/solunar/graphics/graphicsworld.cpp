#include "graphicspch.h"
#include "graphics/graphicsworld.h"
#include "graphics/mesh.h"
#include "engine/camera.h"

namespace solunar {


	GraphicsWorld::GraphicsWorld()
	{
	}

	GraphicsWorld::~GraphicsWorld()
	{
	}

	void GraphicsWorld::onWorldSet(World* world)
	{
	}
	
	void GraphicsWorld::addDrawable(DrawableComponent* drawable)
	{
		Assert2(drawable, "Cannot add null ptr drawable component.");
		m_drawables.push_back(drawable);
	}
	
	void GraphicsWorld::removeDrawable(DrawableComponent* drawable)
	{
		Assert2(drawable, "Cannot remove null ptr drawable component.");
		
		auto it = std::find(m_drawables.begin(), m_drawables.end(), drawable);
		if (it != m_drawables.end())
		{
			Assert((*it) && "DrawableComponent is null ptr.");
			m_drawables.erase(it);
		}
	}

	void GraphicsWorld::updateDrawables()
	{
		//Camera* camera = CameraProxy::getInstance();

		//std::sort(m_drawables.begin(), m_drawables.end(), [camera](DrawableComponent* dc1, DrawableComponent* dc2)
		//{
		//	if (!dc1->isA(MeshComponent::getStaticTypeInfo()) ||
		//		!dc2->isA(MeshComponent::getStaticTypeInfo()))
		//		return false;


		//	float length1 = glm::length(camera->getPosition() - dc1->m_node->getPosition());
		//	float length2 = glm::length(camera->getPosition() - dc2->m_node->getPosition());

		//	return length1 < length2;
		//});
	}
}