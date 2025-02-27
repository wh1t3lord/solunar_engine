#include "core/memory/mallocallocator.h"

#include <malloc.h>

namespace solunar
{

void* MallocAllocator::allocate(size_t size, size_t align)
{
	return _aligned_malloc(size, align);
}

void MallocAllocator::free(void* ptr)
{
	return _aligned_free(ptr);
}

}