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
		DECLARE_OBJECT(MeshComponent);
	public:
		static void RegisterObject();

	public:
		MeshComponent();
		~MeshComponent();

		virtual void LoadXML(tinyxml2::XMLElement& element) override;
		virtual void SaveXML(tinyxml2::XMLElement& element) override;

		virtual void OnWorldSet(World* world) override;

		virtual void OnEntityRemove() override;

		void LoadModel(const std::string& filename);

		virtual void Render();

		std::shared_ptr<ModelBase> LockModel();
		std::weak_ptr<ModelBase> getModel();

		bool IsCastingShadow() { return m_castShadows; }

	protected:
		std::weak_ptr<ModelBase> m_model;
		bool m_castShadows;
	};

	class AnimatedMeshComponent : public MeshComponent
	{
		DECLARE_OBJECT(AnimatedMeshComponent);
	public:
		AnimatedMeshComponent();
		~AnimatedMeshComponent();

		void LoadXML(tinyxml2::XMLElement& element) override;
		void LoadModel(const std::string& filename);

		void Render() override;

		bool HasSkin();
	};

}

#endif