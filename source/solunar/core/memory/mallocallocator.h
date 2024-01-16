#ifndef CORE_MALLOCALLOCATOR_H
#define CORE_MALLOCALLOCATOR_H

#include "core/memory/allocator.h"

namespace engine
{

class MallocAllocator : public IAllocator
{
public:
	void* allocate(size_t size, size_t align) override;
	void free(void* ptr) override;
};

}

#endif // !CORE_MALLOCALLOCATOR_H
