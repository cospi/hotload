#include <common/memory/allocation.hh>

#include <cassert>

Allocation::Allocation(IAllocator &allocator)
	: allocator_(allocator)
	, memory_(nullptr)
{ }

Allocation::~Allocation()
{
	if (void *const memory = memory_; memory != nullptr) {
		allocator_.free(memory);
	}
}

bool Allocation::init(const std::size_t size)
{
	assert(memory_ == nullptr);
	assert(size > 0);

	void *const memory = allocator_.allocate(size);
	if (memory == nullptr) {
		return false;
	}

	memory_ = memory;
	return true;
}

void *Allocation::get_memory() const
{
	assert(memory_ != nullptr);

	return memory_;
}
