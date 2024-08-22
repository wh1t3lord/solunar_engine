#include "graphicspch.h"
#include "graphics/mesh.h"
#include "graphics/debugrenderer.h"
#include "graphics/renderer.h"
#include "graphics/animatedmodel.h"

#include "engine/entity/world.h"

namespace solunar {

	IMPLEMENT_OBJECT(MeshComponent, Component);

	void MeshComponent::RegisterObject()
	{
		g_typeManager->RegisterObject<MeshComponent>();
	}

	MeshComponent::MeshComponent()
	{
	}

	MeshComponent::~MeshComponent()
	{
		m_model.reset();
	}

	void MeshComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* modelElement = element.FirstChildElement("Model");
		if (modelElement)
		{
			const tinyxml2::XMLAttribute* filenameAttribute = modelElement->FindAttribute("filename");
			if (filenameAttribute && strlen( filenameAttribute->Value())>0)
			{
				std::string filename = filenameAttribute->Value();
				m_model = g_contentManager->LoadObject<ModelBase>(filename);
			}
			else
			{
				Core::Msg("WARNING: MeshComponent at entity(0x%p) has empty filename attribute in Model element!", GetEntity());
			}
		}
	}

	void MeshComponent::SaveXML(tinyxml2::XMLElement& element)
	{
	}

	void MeshComponent::OnWorldSet(World* world)
	{
		world->GetGraphicsWorld()->AddMesh(this);
	}

	void MeshComponent::OnEntityRemove()
	{
		if (GetWorld())
			GetWorld()->GetGraphicsWorld()->RemoveMesh(this);
	}

	void MeshComponent::Render()
	{
#ifndef FINAL_BUILD
		BoundingBox boundingBox = GetEntity()->GetBoundingBox();
		g_debugRender.drawBoundingBox(boundingBox, glm::vec3(1.f));
#endif // !FINAL_BUILD
	}

	void MeshComponent::LoadModel(const std::string& filename)
	{
		m_model = g_contentManager->LoadObject<ModelBase>(filename);
	}

	std::shared_ptr<ModelBase> MeshComponent::lockModel()
	{
		return m_model.lock();
	}

	std::weak_ptr<ModelBase> MeshComponent::getModel()
	{
		return m_model;
	}

	// animated mesh component

	IMPLEMENT_OBJECT(AnimatedMeshComponent, MeshComponent);

	AnimatedMeshComponent::AnimatedMeshComponent()
	{
	}

	AnimatedMeshComponent::~AnimatedMeshComponent()
	{
		// m_model.reset();
	}

	void AnimatedMeshComponent::LoadXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* modelElement = element.FirstChildElement("Model");
		if (modelElement)
		{
			const tinyxml2::XMLAttribute* filenameAttribute = modelElement->FindAttribute("filename");
			if (filenameAttribute && strlen(filenameAttribute->Value()) > 0)
			{
				std::string filename = filenameAttribute->Value();
				m_model = g_contentManager->LoadObject<AnimatedModel>(filename);

				// guarantee cast, trust me 
				std::weak_ptr<AnimatedModel> animatedModel = dynamicCastWeakPtr<AnimatedModel, ModelBase>(m_model);

				Entity* entity = GetEntity();
				if (entity)
					entity->SetBoundingBox(animatedModel.lock()->GetBoundingBox()); // #TODO: too shitty usage, lock and unlock on stack :(
			}
			else
			{
				Core::Msg("WARNING: AnimatedMeshComponent at entity(0x%p) has empty filename attribute in Model element!", GetEntity());
			}
		}
	}

	void AnimatedMeshComponent::LoadModel(const std::string& filename)
	{
		if (!filename.empty())
		{
			m_model = g_contentManager->LoadObject<AnimatedModel>(filename);

			// guarantee cast, trust me 
			std::weak_ptr<AnimatedModel> animatedModel = dynamicCastWeakPtr<AnimatedModel, ModelBase>(m_model);

			Entity* entity = GetEntity();
			if (entity)
				entity->SetBoundingBox(animatedModel.lock()->GetBoundingBox()); // #TODO: too shitty usage, lock and unlock on stack :(
		}
		else
		{
			Core::Msg("WARNING: AnimatedMeshComponent at entity(0x%p) has empty filename!", GetEntity());
		}
	}
}