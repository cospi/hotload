#include "game.hh"

#include <common/render/shader_pipeline.hh>

static const float SPEED = 128.0f;

Game::Game(Platform &platform)
	: platform_(platform)
	, shader_pipeline_(platform.shader_pipeline_factory)
	, projection_uniform_(-1)
	, texture_(platform.texture_factory)
	, sprite_batch_(platform.sprite_batch_factory)
	, projection_(Matrix4::ortho(0.0f, 640.0f, 0.0f, 480.0f, 0.0f, -1000.0f))
	, position_(0.0f, 0.0f)
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

	if (!sprite_batch_.init(64)) {
		return false;
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

	SpriteBatch &sprite_batch = sprite_batch_;
	if (sprite_batch.begin_push()) {
		sprite_batch.push_sprite(Sprite(
			Rect(position_, Vector2(64.0f, 64.0f)),
			Rect(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f))
		));
		sprite_batch.end_push();
		render_command_buffer.draw_sprite_batch(sprite_batch);
	}
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
	const bool up = up_;
	const bool down = down_;
	const bool left = left_;
	const bool right = right_;
	Vector2 delta_position(
		(left && !right) ? -1.0f : (right && !left) ? 1.0f : 0.0f,
		(down && !up) ? -1.0f : (up && !down) ? 1.0f : 0.0f
	);
	if ((delta_position.x != 0.0f) || (delta_position.y != 0.0f)) {
		delta_position.normalize();
		delta_position *= SPEED * delta_time;
		position_ += delta_position;
	}
}
