#include <common/platform/platform.hh>

Platform::Platform(ILogger &in_logger, IAllocator &in_allocator, IFileSystem &in_file_system)
	: logger(in_logger)
	, allocator(in_allocator)
	, file_system(in_file_system)
{ }
