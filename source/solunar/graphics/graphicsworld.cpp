#include "graphicspch.h"
#include "graphics/graphicsworld.h"
#include "graphics/mesh.h"

#include "core/timer.h"

#include "engine/camera.h"

#include <algorithm>

namespace solunar {

	IMPLEMENT_OBJECT(GraphicsWorld, Object);

	void GraphicsWorld::RegisterObject()
	{
		g_typeManager->RegisterObject<GraphicsWorld>();
	}

	GraphicsWorld::GraphicsWorld()
	{
	}

	GraphicsWorld::~GraphicsWorld()
	{
	}

	void GraphicsWorld::AddMesh(MeshComponent* mesh)
	{
		Assert2(mesh, "Cannot add nullptr mesh component.");
		m_meshes.push_back(mesh);
	}
	
	void GraphicsWorld::RemoveMesh(MeshComponent* mesh)
	{
		Assert2(mesh, "Cannot remove nullptr mesh component.");
		
		auto it = std::find(m_meshes.begin(), m_meshes.end(), mesh);
		if (it != m_meshes.end())
		{
			Assert((*it) && "MeshComponent is nullptr.");
			m_meshes.erase(it);
		}
	}

	void GraphicsWorld::Update()
	{
		Camera* camera = CameraProxy::GetInstance();

		std::sort(m_meshes.begin(), m_meshes.end(), [camera](MeshComponent* dc1, MeshComponent* dc2)
		{
			if (!dc1->IsA(MeshComponent::GetStaticTypeInfo()) ||
				!dc2->IsA(MeshComponent::GetStaticTypeInfo()))
				return false;

			glm::vec3 pos1 = camera->GetPosition() - dc1->GetEntity()->GetPosition();
			glm::vec3 pos2 = camera->GetPosition() - dc2->GetEntity()->GetPosition();

			float sqrdist1 = glm::dot(pos1, pos1);
			float sqrdist2 = glm::dot(pos2, pos2);

			return sqrdist1 > sqrdist2;
		});
	}
}