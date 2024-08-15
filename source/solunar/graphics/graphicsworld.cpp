#include "graphicspch.h"
#include "graphics/graphicsworld.h"
#include "graphics/mesh.h"
#include "engine/camera.h"

#include <algorithm>

namespace solunar {


	GraphicsWorld::GraphicsWorld()
	{
	}

	GraphicsWorld::~GraphicsWorld()
	{
	}

	void GraphicsWorld::OnWorldSet(World* world)
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

			float length1 = glm::length(camera->GetPosition() - dc1->GetEntity()->GetPosition());
			float length2 = glm::length(camera->GetPosition() - dc2->GetEntity()->GetPosition());

			return length1 < length2;
		});
	}
}