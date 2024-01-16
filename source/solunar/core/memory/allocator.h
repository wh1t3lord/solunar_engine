#ifndef CORE_ALLOCATOR_H
#define CORE_ALLOCATOR_H

namespace engine
{

class IAllocator
{
public:
	virtual ~IAllocator() {}

	virtual void* allocate(size_t size, size_t align) = 0;
	virtual void free(void* ptr) = 0;
};

}

#endif // !CORE_ALLOCATOR_H
