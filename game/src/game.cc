#include "game.hh"

#include <common/render/shader_pipeline.hh>

Game::Game(Platform &platform)
	: platform_(platform)
	, shader_pipeline_(platform.shader_pipeline_factory)
	, projection_uniform_(-1)
	, texture_(platform.texture_factory)
	, mesh_(platform.mesh_factory)
	, projection_(Matrix4::ortho(0.0f, 640.0f, 0.0f, 480.0f, 0.0f, -1000.0f))
{ }

bool Game::init()
{
	{
		ShaderPipeline &shader_pipeline = shader_pipeline_;
		if (
			!shader_pipeline_.init("test")
			|| !shader_pipeline.try_get_uniform("u_projection", &projection_uniform_)
		) {
			return false;
		}
	}

	{
		Image image(platform_.logger, platform_.allocator, platform_.file_system);
		if (!image.init_from_tga("test") || !texture_.init(image)) {
			return false;
		}
	}

	{
		const Vertex vertices[4] = {
			Vertex(Vector3(160.0f, 120.0f, 0.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector3(480.0f, 120.0f, 0.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector3(480.0f, 360.0f, 0.0f), Vector2(1.0f, 1.0f)),
			Vertex(Vector3(160.0f, 360.0f, 0.0f), Vector2(0.0f, 1.0f)),
		};
		const std::uint16_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		if (!mesh_.init(vertices, 4, indices, 6)) {
			return false;
		}
	}

	return true;
}

void Game::tick()
{
	RenderCommandBuffer &render_command_buffer = platform_.render_command_buffer;
	render_command_buffer.clear(Color(0.0f, 0.5f, 1.0f, 1.0f));
	render_command_buffer.set_shader_pipeline(shader_pipeline_);
	render_command_buffer.set_uniform_matrix4(projection_uniform_, projection_);
	render_command_buffer.draw_mesh(mesh_);
}
