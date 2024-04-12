#ifndef CORE_MEMORYMANAGER_H
#define CORE_MEMORYMANAGER_H

#include "core/debug.h"
#include "core/memory/allocator.h"

#include <new>

namespace engine
{

class CORE_API MemoryManager
{
public:
	static void initialize();
	static void shutdown();
	
	static IAllocator* getDefaultAllocator();

private:
	static IAllocator* ms_pDefaultAllocator;

};

template <typename T, typename... Args>
T* mem_new(Args... args)
{
	IAllocator* pAllocator = MemoryManager::getDefaultAllocator();
	return new(pAllocator->allocate(sizeof(T), __alignof(T))) T(args...);
}

template <typename T>
void mem_delete(T*& ptr)
{
	IAllocator* pAllocator = MemoryManager::getDefaultAllocator();
	if (ptr)
	{
		ptr->~T();
		pAllocator->free(ptr);
		//ptr = nullptr;
	}
}

template <typename T>
T* mem_array(size_t length)
{
	IAllocator* pAllocator = MemoryManager::getDefaultAllocator();

	Assert(length != 0);
	uint8_t headerSize = sizeof(size_t) / sizeof(T);

	if (sizeof(size_t) % sizeof(T) > 0) headerSize += 1;

	//Allocate extra space to store array length in the bytes before the array 
	T* p = ((T*)pAllocator->allocate(sizeof(T) * (length + headerSize), __alignof(T))) + headerSize;
	*(((size_t*)p) - 1) = length;

	for (size_t i = 0; i < length; i++)
		new (&p) T;

	return p;
}

template <typename T>
void mem_free_array(T* array)
{
	Assert(array != nullptr);
	size_t length = *(((size_t*)array) - 1);

	//for (size_t i = 0; i < length; i++) array.~T();

	//Calculate how much extra memory was allocated to store the length before the array 
	uint8_t headerSize = sizeof(size_t) / sizeof(T);
	if (sizeof(size_t) % sizeof(T) > 0)
		headerSize += 1;

	IAllocator* pAllocator = MemoryManager::getDefaultAllocator();
	pAllocator->free(array - headerSize);
}
		
}

#endif