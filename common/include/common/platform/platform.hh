#ifndef HOTLOAD_COMMON_PLATFORM_PLATFORM_HH_
#define HOTLOAD_COMMON_PLATFORM_PLATFORM_HH_

#include "../file_system/i_file_system.hh"
#include "../log/i_logger.hh"
#include "../memory/i_allocator.hh"
#include "../render/render_command_buffer.hh"

class Platform final {
public:
	Platform(
		ILogger &in_logger,
		IAllocator &in_allocator,
		IFileSystem &in_file_system,
		RenderCommandBuffer &in_render_command_buffer
	);
	~Platform() = default;

	Platform(const Platform &) = delete;
	Platform &operator=(const Platform &) = delete;

	ILogger &logger;
	IAllocator &allocator;
	IFileSystem &file_system;
	RenderCommandBuffer &render_command_buffer;
};

#endif // HOTLOAD_COMMON_PLATFORM_PLATFORM_HH_
