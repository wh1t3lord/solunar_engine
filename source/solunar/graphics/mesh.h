#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include "core/math/boundingbox.h"

namespace engine {

	//class MeshResource : public Content
	//{
	//	ImplementObject(MeshResource, Content);
	//public:
	//	static void registerObject();

	//public:
	//	MeshResource();
	//	~MeshResource();
	//};

	class MeshComponent : public Component
	{
		ImplementObject(MeshComponent, Component);
	public:
		static void registerObject();

	public:
		MeshComponent();
		~MeshComponent();

		void loadXML(tinyxml2::XMLElement& element) override;
		void saveXML(tinyxml2::XMLElement& element) override;

		virtual void render();

	private:
		BoundingBox m_boundingBox;
	};

}

#endif