#include "core/core.h"
#include "core/timer.h"
#include "core/utils/logger.h"
#include "core/memory/memorymanager.h"
#include "core/file/filesystem_win32.h"
#include "core/file/filesystem_pack.h"
#include "core/file/contentmanager.h"
#include "core/object/typemanager.h"
#include "core/object/object.h"
#include "core/object/serializableobject.h"
#include "core/object/propertymanager.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

namespace solunar
{
	
void RegisterCoreTypes()
{
	TypeManager::GetInstance()->RegisterObject<Object>();
	TypeManager::GetInstance()->RegisterObject<SerializableObject>();
}

void Core::Init()
{
	// Initialize random
	srand(time(NULL));

	Logger::Init();
	Logger::LogPrint("Core builted at %s %s", __TIME__, __DATE__);

	MemoryManager::Initialize();

	Timer::GetInstance()->Init();

	// Allocate native filesystem
	g_fileSystem = mem_new<FileSystem_Win32>();

	g_fileSystemPack.initialize();

	g_contentManager = mem_new<ContentManager>();
	g_contentManager->Init();

	// register core types
	RegisterCoreTypes();
}

void Core::Shutdown()
{
	if (g_contentManager)
	{
		mem_delete(g_contentManager);
		g_contentManager = nullptr;
	}

	g_fileSystemPack.shutdown();

	if (g_fileSystem)
	{
		mem_delete(g_fileSystem);
		g_fileSystem = nullptr;
	}

	PropertyManager::GetInstance()->Shutdown();

	MemoryManager::Shutdown();

	Logger::Shutdown();
}

}