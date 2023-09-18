#ifndef HOTLOAD_COMMON_MEMORY_ALLOCATION_HH_
#define HOTLOAD_COMMON_MEMORY_ALLOCATION_HH_

#include "i_allocator.hh"

class Allocation final {
public:
	explicit Allocation(IAllocator &allocator);
	~Allocation();

	Allocation(const Allocation &) = delete;
	Allocation &operator=(const Allocation &) = delete;

	bool init(std::size_t size);

	void *get_memory() const;

private:
	IAllocator &allocator_;
	void *memory_;
};

#endif // HOTLOAD_COMMON_MEMORY_ALLOCATION_HH_
