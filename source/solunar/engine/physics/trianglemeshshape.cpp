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

	FileHandle file = g_fileSystem->Create(buffer);

	/////////////////////////////////////////////////////
	// Write header
	uint64_t ullMagic = get_cm_magic();
	g_fileSystem->Write(file, &ullMagic, sizeof(ullMagic));

	float fVersion = CM_VERSION;
	g_fileSystem->Write(file, &fVersion, sizeof(fVersion));

	g_fileSystem->Close(file);
}

/////////////////////////////////////////////////////////////////////

TriangleMeshShapeComponent::TriangleMeshShapeComponent()
{
}

TriangleMeshShapeComponent::~TriangleMeshShapeComponent()
{
}

void TriangleMeshShapeComponent::RegisterObject()
{
	g_typeManager->RegisterObject<TriangleMeshShapeComponent>();
}

void TriangleMeshShapeComponent::LoadXML(tinyxml2::XMLElement& element)
{
	ShapeComponent::LoadXML(element);
}

void TriangleMeshShapeComponent::SaveXML(tinyxml2::XMLElement& element)
{
	ShapeComponent::SaveXML(element);
}

void TriangleMeshShapeComponent::CreateShapeInternal()
{
	TriangleMeshServer::GetInstance()->saveCollision("test");
}

}