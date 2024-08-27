#include "core/memory/mallocallocator.h"

#include <stdlib.h>

#ifdef WIN32
#include <malloc.h>
#endif // WIN32

namespace solunar
{

#ifdef WIN32
void* MallocAllocator::allocate(size_t size, size_t align)
{
	return _aligned_malloc(size, align);
}

void MallocAllocator::free(void* ptr)
{
	return _aligned_free(ptr);
}
#else
void* MallocAllocator::allocate(size_t size, size_t align)
{
	return malloc(size);
}

void MallocAllocator::free(void* ptr)
{
	return free(ptr);
}
#endif

}