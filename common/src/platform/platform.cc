#include <common/platform/platform.hh>

Platform::Platform(
	ILogger &in_logger,
	IAllocator &in_allocator,
	IFileSystem &in_file_system,
	IShaderPipelineFactory &in_shader_pipeline_factory,
	ITextureFactory &in_texture_factory,
	IMeshFactory &in_mesh_factory,
	ISpriteBatchFactory &in_sprite_batch_factory,
	RenderCommandBuffer &in_render_command_buffer
)
	: logger(in_logger)
	, allocator(in_allocator)
	, file_system(in_file_system)
	, shader_pipeline_factory(in_shader_pipeline_factory)
	, texture_factory(in_texture_factory)
	, mesh_factory(in_mesh_factory)
	, sprite_batch_factory(in_sprite_batch_factory)
	, render_command_buffer(in_render_command_buffer)
{ }
