#include <common/platform/platform.hh>

Platform::Platform(
	ILogger &in_logger,
	IAllocator &in_allocator,
	IFileSystem &in_file_system,
	RenderCommandBuffer &in_render_command_buffer
)
	: logger(in_logger)
	, allocator(in_allocator)
	, file_system(in_file_system)
	, render_command_buffer(in_render_command_buffer)
{ }
