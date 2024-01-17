#ifndef ENGINE_PHYSICS_TRIANGLEMESHSHAPE_H
#define ENGINE_PHYSICS_TRIANGLEMESHSHAPE_H

#include "engine/physics/shapescomponent.h"

namespace engine
{

#define CM_MAGIC	"CollisionFile"
#define CM_VERSION	float(0.1f)

enum class CollisionShapeType : uint8_t
{
	Box,
	Cyllinder,
	Sphere,
	Trimesh
};

struct CollisionShapeHeader
{
	// trimesh
	uint32_t vertices_count;
	uint32_t indecies_count;

	// box
	glm::vec3 box_min;
	glm::vec3 box_max;
};

//! Server which store and restore trimesh shapes for bullet
class TriangleMeshServer : public Singleton<TriangleMeshServer>
{
public:
	TriangleMeshServer();
	~TriangleMeshServer();

	void saveCollision(const char* modelfilename);

private:
	struct TrimeshEntry
	{
		std::string m_filename;
		std::string m_modelFilename;
		btTriangleMesh* m_trimesh;
	};

	std::vector<TrimeshEntry> m_trimeshes;
};

//! Trimesh collision shape
class TriangleMeshShapeComponent : public ShapeComponent
{
	ImplementObject(TriangleMeshShapeComponent, ShapeComponent);
public:
	TriangleMeshShapeComponent();
	~TriangleMeshShapeComponent();

	static void registerObject();

	void loadXML(tinyxml2::XMLElement& element) override;
	void saveXML(tinyxml2::XMLElement& element) override;

private:
	void createShapeInternal() override;
};

}

#endif // !ENGINE_PHYSICS_TRIANGLEMESHSHAPE_H
