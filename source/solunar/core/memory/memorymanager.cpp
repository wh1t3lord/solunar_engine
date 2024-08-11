#include "core/core.h"
#include "core/memory/memorymanager.h"
#include "core/memory/mallocallocator.h"

namespace solunar
{
	
IAllocator* MemoryManager::ms_pDefaultAllocator = nullptr;

void MemoryManager::Initialize()
{
	static MallocAllocator s_mallocAllocator;
	ms_pDefaultAllocator = &s_mallocAllocator;
}

void MemoryManager::Shutdown()
{
	ms_pDefaultAllocator = nullptr;
}

IAllocator* MemoryManager::getDefaultAllocator()
{
	return ms_pDefaultAllocator;
}

}