#ifndef CORE_ALLOCATOR_H
#define CORE_ALLOCATOR_H

#include "core/platform/core_platform.h"

namespace engine
{

class CORE_API IAllocator
{
public:
	virtual ~IAllocator() {}

	virtual void* allocate(size_t size, size_t align) = 0;
	virtual void free(void* ptr) = 0;
};

}

#endif // !CORE_ALLOCATOR_H
