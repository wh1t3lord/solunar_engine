#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include "graphics/model.h"

namespace solunar {

	//class MeshResource : public Content
	//{
	//	IMPLEMENT_OBJECT(MeshResource, Content);
	//public:
	//	static void RegisterObject();

	//public:
	//	MeshResource();
	//	~MeshResource();
	//};

	class MeshComponent : public Component
	{
		IMPLEMENT_OBJECT(MeshComponent, Component);
	public:
		static void RegisterObject();

	public:
		MeshComponent();
		~MeshComponent();

		virtual void LoadXML(tinyxml2::XMLElement& element) override;
		virtual void SaveXML(tinyxml2::XMLElement& element) override;

		virtual void OnWorldSet(World* world) override;

		void LoadModel(const std::string& filename);

		virtual void Render();

		std::shared_ptr<ModelBase> lockModel();
		std::weak_ptr<ModelBase> getModel();

	protected:
		std::weak_ptr<ModelBase> m_model;
	};

	class AnimatedMeshComponent : public MeshComponent
	{
		IMPLEMENT_OBJECT(AnimatedMeshComponent, MeshComponent);
	public:
		AnimatedMeshComponent();
		~AnimatedMeshComponent();

		void LoadXML(tinyxml2::XMLElement& element) override;
		void LoadModel(const std::string& filename);
	};

}

#endif