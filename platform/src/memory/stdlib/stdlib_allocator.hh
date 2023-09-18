#ifndef HOTLOAD_PLATFORM_MEMORY_STDLIB_STDLIB_ALLOCATOR_HH_
#define HOTLOAD_PLATFORM_MEMORY_STDLIB_STDLIB_ALLOCATOR_HH_

#include <common/log/i_logger.hh>
#include <common/memory/i_allocator.hh>

class StdlibAllocator final : public IAllocator {
public:
	explicit StdlibAllocator(ILogger &logger);
	~StdlibAllocator() override = default;

	StdlibAllocator(const StdlibAllocator &) = delete;
	StdlibAllocator &operator=(const StdlibAllocator &) = delete;

	void *allocate(std::size_t size) override;
	void free(void *memory) override;

private:
	ILogger &logger_;
};

#endif // HOTLOAD_PLATFORM_MEMORY_STDLIB_STDLIB_ALLOCATOR_HH_
