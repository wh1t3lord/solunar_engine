#include "enginepch.h"
#include "engine/physics/trianglemeshshape.h"

#include "core/file/filesystem.h"

namespace solunar
{

// not good but normal for our needs
uint64_t generate_fast_hash(const char* str)
{
	uint64_t ullHash = 0ll;
	for (; *str; str++)
		ullHash += uint64_t(*str);
	return (ullHash);
}

uint64_t get_cm_magic()
{
	// calculate it only once
	static uint64_t ullMagic = generate_fast_hash(CM_MAGIC);
	return (ullMagic);
}

static TriangleMeshServer s_triangleMeshServer;

TriangleMeshServer::TriangleMeshServer()
{
}

TriangleMeshServer::~TriangleMeshServer()
{
}

void TriangleMeshServer::saveCollision(const char* modelfilename)
{
	char buffer[260];
	snprintf(buffer, sizeof(buffer), "data/models/%s.cm", modelfilename);

	FileHandle file = g_fileSystem->create(buffer);

	/////////////////////////////////////////////////////
	// Write header
	uint64_t ullMagic = get_cm_magic();
	g_fileSystem->write(file, &ullMagic, sizeof(ullMagic));

	float fVersion = CM_VERSION;
	g_fileSystem->write(file, &fVersion, sizeof(fVersion));

	g_fileSystem->close(file);
}

/////////////////////////////////////////////////////////////////////

TriangleMeshShapeComponent::TriangleMeshShapeComponent()
{
}

TriangleMeshShapeComponent::~TriangleMeshShapeComponent()
{
}

void TriangleMeshShapeComponent::registerObject()
{
	g_typeManager->registerObject<TriangleMeshShapeComponent>();
}

void TriangleMeshShapeComponent::loadXML(tinyxml2::XMLElement& element)
{
	ShapeComponent::loadXML(element);
}

void TriangleMeshShapeComponent::saveXML(tinyxml2::XMLElement& element)
{
	ShapeComponent::saveXML(element);
}

void TriangleMeshShapeComponent::createShapeInternal()
{
	TriangleMeshServer::getInstance()->saveCollision("test");
}

}