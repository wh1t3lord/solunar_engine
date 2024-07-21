#include "graphicspch.h"
#include "graphics/mesh.h"
#include "graphics/debugrenderer.h"
#include "graphics/renderer.h"
#include "graphics/animatedmodel.h"

namespace engine {

	void MeshComponent::registerObject()
	{
		g_typeManager->registerObject<MeshComponent>();
	}

	MeshComponent::MeshComponent()
	{
	}

	MeshComponent::~MeshComponent()
	{
		m_model.reset();
	}

	void MeshComponent::loadXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* modelElement = element.FirstChildElement("Model");
		if (modelElement)
		{
			const tinyxml2::XMLAttribute* filenameAttribute = modelElement->FindAttribute("filename");
			if (filenameAttribute && strlen( filenameAttribute->Value())>0)
			{
				std::string filename = filenameAttribute->Value();
				m_model = g_contentManager->loadObject<ModelBase>(filename);
			}
			else
			{
				Core::msg("WARNING: MeshComponent at entity(0x%p) has empty filename attribute in Model element!", getEntity());
			}
		}
	}

	void MeshComponent::saveXML(tinyxml2::XMLElement& element)
	{
	}

	void MeshComponent::render()
	{
#ifndef FINAL_BUILD
		BoundingBox boundingBox = getEntity()->getBoundingBox();
		g_debugRender.drawBoundingBox(boundingBox, glm::vec3(1.f));
#endif // !FINAL_BUILD
	}

	std::shared_ptr<ModelBase> MeshComponent::lockModel()
	{
		return m_model.lock();
	}

	// animated mesh component

	AnimatedMeshComponent::AnimatedMeshComponent()
	{
	}

	AnimatedMeshComponent::~AnimatedMeshComponent()
	{
		// m_model.reset();
	}

	void AnimatedMeshComponent::loadXML(tinyxml2::XMLElement& element)
	{
		tinyxml2::XMLElement* modelElement = element.FirstChildElement("Model");
		if (modelElement)
		{
			const tinyxml2::XMLAttribute* filenameAttribute = modelElement->FindAttribute("filename");
			if (filenameAttribute && strlen(filenameAttribute->Value()) > 0)
			{
				std::string filename = filenameAttribute->Value();
				m_model = g_contentManager->loadObject<AnimatedModel>(filename);

				// guarantee cast, trust me 
				std::weak_ptr<AnimatedModel> animatedModel = dynamicCastWeakPtr<AnimatedModel, ModelBase>(m_model);

				Entity* entity = getEntity();
				if (entity)
					entity->setBoundingBox(animatedModel.lock()->getBoundingBox()); // #TODO: too shitty usage, lock and unlock on stack :(
			}
			else
			{
				Core::msg("WARNING: AnimatedMeshComponent at entity(0x%p) has empty filename attribute in Model element!", getEntity());
			}
		}
	}

	void AnimatedMeshComponent::loadModel(const std::string& filename)
	{
		if (!filename.empty())
		{
			m_model = g_contentManager->loadObject<AnimatedModel>(filename);

			// guarantee cast, trust me 
			std::weak_ptr<AnimatedModel> animatedModel = dynamicCastWeakPtr<AnimatedModel, ModelBase>(m_model);

			Entity* entity = getEntity();
			if (entity)
				entity->setBoundingBox(animatedModel.lock()->getBoundingBox()); // #TODO: too shitty usage, lock and unlock on stack :(
		}
		else
		{
			Core::msg("WARNING: AnimatedMeshComponent at entity(0x%p) has empty filename!", getEntity());
		}
	}
}