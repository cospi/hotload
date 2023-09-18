#ifndef HOTLOAD_COMMON_MEMORY_I_ALLOCATOR_HH_
#define HOTLOAD_COMMON_MEMORY_I_ALLOCATOR_HH_

#include <cstddef>

class IAllocator {
public:
	virtual ~IAllocator() = default;

	virtual void *allocate(std::size_t size) = 0;
	virtual void free(void *memory) = 0;
};

#endif // HOTLOAD_COMMON_MEMORY_I_ALLOCATOR_HH_
