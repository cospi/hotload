#include "stdlib_allocator.hh"

#include <cassert>
#include <cinttypes>
#include <cstdlib>

StdlibAllocator::StdlibAllocator(ILogger &logger)
	: logger_(logger)
{ }

void *StdlibAllocator::allocate(const std::size_t size)
{
	assert(size > 0);

	void *const memory = std::malloc(size);
	if (memory == nullptr) {
		logger_.log(LogLevel::ERR, "Allocating %zu bytes of memory failed.", size);
		return nullptr;
	}

	logger_.log(
		LogLevel::INFO,
		"Allocated %zu bytes of memory (%" PRIxPTR ").",
		size,
		reinterpret_cast<std::uintptr_t>(memory)
	);
	return memory;
}

void StdlibAllocator::free(void *const memory)
{
	assert(memory != nullptr);

	std::free(memory);
	logger_.log(LogLevel::INFO, "Freed memory (%" PRIxPTR ").", reinterpret_cast<std::uintptr_t>(memory));
}
