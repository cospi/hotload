#include "game.hh"

#include <common/render/shader_pipeline.hh>

static const float SPEED = 128.0f;

Game::Game(Platform &platform)
	: platform_(platform)
	, shader_pipeline_(platform.shader_pipeline_factory)
	, projection_uniform_(-1)
	, translation_uniform_(-1)
	, texture_(platform.texture_factory)
	, mesh_(platform.mesh_factory)
	, projection_(Matrix4::ortho(0.0f, 640.0f, 0.0f, 480.0f, 0.0f, -1000.0f))
	, position_(0.0f, 0.0f, 0.0f)
	, up_(false)
	, down_(false)
	, left_(false)
	, right_(false)
{ }

bool Game::init()
{
	{
		ShaderPipeline &shader_pipeline = shader_pipeline_;
		if (
			!shader_pipeline_.init("test")
			|| !shader_pipeline.try_get_uniform("u_projection", &projection_uniform_)
			|| !shader_pipeline.try_get_uniform("u_translation", &translation_uniform_)
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
			Vertex(Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector3(64.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector3(64.0f, 64.0f, 0.0f), Vector2(1.0f, 1.0f)),
			Vertex(Vector3(0.0f, 64.0f, 0.0f), Vector2(0.0f, 1.0f)),
		};
		const std::uint16_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		if (!mesh_.init(vertices, 4, indices, 6)) {
			return false;
		}
	}

	return true;
}

void Game::handle_event(const Event &event)
{
	switch (event.type) {
	case EventType::KEY_DOWN:
		handle_key_event(event.event.key_down.key, true);
		break;
	case EventType::KEY_UP:
		handle_key_event(event.event.key_up.key, false);
		break;
	default:
		break;
	}
}

void Game::tick(const float delta_time)
{
	tick_position(delta_time);
	RenderCommandBuffer &render_command_buffer = platform_.render_command_buffer;
	render_command_buffer.clear(Color(0.0f, 0.5f, 1.0f, 1.0f));
	render_command_buffer.set_shader_pipeline(shader_pipeline_);
	render_command_buffer.set_uniform_matrix4(projection_uniform_, projection_);
	render_command_buffer.set_uniform_matrix4(translation_uniform_, Matrix4::translate(position_));
	render_command_buffer.draw_mesh(mesh_);
}

void Game::handle_key_event(const Key key, const bool down)
{
	switch (key) {
	case Key::UP_ARROW:
		up_ = down;
		break;
	case Key::DOWN_ARROW:
		down_ = down;
		break;
	case Key::LEFT_ARROW:
		left_ = down;
		break;
	case Key::RIGHT_ARROW:
		right_ = down;
		break;
	default:
		break;
	}
}

void Game::tick_position(const float delta_time)
{
	Vector3 delta_position(0.0f, 0.0f, 0.0f);
	bool move = false;

	if (up_) {
		delta_position.y += 1.0f;
		move = true;
	}
	if (down_) {
		delta_position.y -= 1.0f;
		move = true;
	}
	if (left_) {
		delta_position.x -= 1.0f;
		move = true;
	}
	if (right_) {
		delta_position.x += 1.0f;
		move = true;
	}

	if (move) {
		delta_position.normalize();
		delta_position *= SPEED * delta_time;
		position_ += delta_position;
	}
}
