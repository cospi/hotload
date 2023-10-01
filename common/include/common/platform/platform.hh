#ifndef HOTLOAD_COMMON_PLATFORM_PLATFORM_HH_
#define HOTLOAD_COMMON_PLATFORM_PLATFORM_HH_

#include "../file_system/i_file_system.hh"
#include "../log/i_logger.hh"
#include "../memory/i_allocator.hh"
#include "../render/render_command_buffer.hh"
#include "../render/i_mesh_factory.hh"
#include "../render/i_shader_pipeline_factory.hh"
#include "../render/i_sprite_batch_factory.hh"
#include "../render/i_texture_factory.hh"

class Platform final {
public:
	Platform(
		ILogger &in_logger,
		IAllocator &in_allocator,
		IFileSystem &in_file_system,
		IShaderPipelineFactory &in_shader_pipeline_factory,
		ITextureFactory &in_texture_factory,
		IMeshFactory &in_mesh_factory,
		ISpriteBatchFactory &in_sprite_batch_factory,
		RenderCommandBuffer &in_render_command_buffer
	);
	~Platform() = default;

	Platform(const Platform &) = delete;
	Platform &operator=(const Platform &) = delete;

	ILogger &logger;
	IAllocator &allocator;
	IFileSystem &file_system;
	IShaderPipelineFactory &shader_pipeline_factory;
	ITextureFactory &texture_factory;
	IMeshFactory &mesh_factory;
	ISpriteBatchFactory &sprite_batch_factory;
	RenderCommandBuffer &render_command_buffer;
};

#endif // HOTLOAD_COMMON_PLATFORM_PLATFORM_HH_
