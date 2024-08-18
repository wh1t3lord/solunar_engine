#ifndef GRAPHICSWORLD_H
#define GRAPHICSWORLD_H

#include "graphics/lightmanager.h"

namespace solunar {
	
	class MeshComponent;
	
	class GraphicsWorld : public Object
	{
		IMPLEMENT_OBJECT(GraphicsWorld, Object);
	public:
		static void RegisterObject();
		
	public:
		GraphicsWorld();
		~GraphicsWorld();

		void AddMesh(MeshComponent* mesh);
		void RemoveMesh(MeshComponent* mesh);

		LightManager* GetLightManager() { return &m_lightManager; }

		const std::vector<MeshComponent*>& GetMeshes() { return m_meshes; }

		void Update();
		
	private:
		std::vector<MeshComponent*> m_meshes;

		LightManager m_lightManager;
	};
	
}

#endif