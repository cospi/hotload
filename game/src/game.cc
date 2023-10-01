#include "game.hh"

#include <common/render/geometry.hh>
#include <common/render/render_buffer.hh>
#include <common/render/shader_pipeline.hh>
#include <common/render/text.hh>
#include <common/math/constants.hh>

static const float FOV = 60.0f * DEG_TO_RAD;
static const float WIDTH = 640.0f;
static const float HEIGHT = 480.0f;
static const float ASPECT_RATIO = WIDTH / HEIGHT;
static const float NEAR = 0.0f;
static const float FAR = -1000.0f;

static const Vector3 CAMERA_POSITION = Vector3(0.0f, 2.0f, 5.0f);

static const float MOVE_SPEED = 5.0f;
static const float JUMP_SPEED = 10.0f;
static const float GRAVITY = -20.0f;

Game::Game(Platform &platform)
	: platform_(platform)
	, font_(platform.logger, platform.allocator, platform.file_system, platform.texture_factory)
	, sprite_batch_(platform.sprite_batch_factory)
	, sprite_batch_shader_pipeline_(platform.shader_pipeline_factory)
	, sprite_batch_projection_uniform_(-1)
	, sprite_batch_projection_(Matrix4::ortho(0.0f, WIDTH, 0.0f, HEIGHT, NEAR, FAR))
	, mesh_(platform.mesh_factory)
	, mesh_shader_pipeline_(platform.shader_pipeline_factory)
	, mesh_projection_uniform_(-1)
	, mesh_translation_uniform_(-1)
	, mesh_texture_(platform.texture_factory)
	, mesh_projection_(Matrix4::perspective(FOV, ASPECT_RATIO, NEAR, FAR))
	, mesh_position_(0.0f, 0.0f, 0.0f)
	, velocity_(0.0f, 0.0f, 0.0f)
	, left_(false)
	, right_(false)
	, jump_(false)
{ }

bool Game::init()
{
	if (!font_.init("font")) {
		return false;
	}

	if (!sprite_batch_.init(64)) {
		return false;
	}

	{
		ShaderPipeline &shader_pipeline = sprite_batch_shader_pipeline_;
		if (
			!shader_pipeline.init("sprite")
			|| !shader_pipeline.try_get_uniform("u_projection", &sprite_batch_projection_uniform_)
		) {
			return false;
		}
	}

	{
		Vertex vertices[CUBE_VERTEX_COUNT];
		std::uint16_t indices[CUBE_INDEX_COUNT];
		geometry_init_cube(vertices, indices, 0.5f);
		if (!mesh_.init(vertices, CUBE_VERTEX_COUNT, indices, CUBE_INDEX_COUNT)) {
			return false;
		}
	}

	{
		ShaderPipeline &shader_pipeline = mesh_shader_pipeline_;
		if (
			!shader_pipeline.init("mesh")
			|| !shader_pipeline.try_get_uniform("u_projection", &mesh_projection_uniform_)
			|| !shader_pipeline.try_get_uniform("u_translation", &mesh_translation_uniform_)
		) {
			return false;
		}
	}

	{
		Platform &platform = platform_;
		Image image(platform.logger, platform.allocator, platform.file_system);
		if (!image.init_from_tga("test") || !mesh_texture_.init(image, TextureFilter::NEAREST)) {
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

	render_command_buffer.clear_render_target(
		RenderBuffer::COLOR | RenderBuffer::DEPTH,
		Color(0.0f, 0.0f, 0.0f, 1.0f)
	);

	render_command_buffer.set_shader_pipeline(mesh_shader_pipeline_);
	render_command_buffer.set_uniform_matrix4(mesh_projection_uniform_, mesh_projection_);
	render_command_buffer.set_uniform_matrix4(
		mesh_translation_uniform_,
		Matrix4::translate(mesh_position_ - CAMERA_POSITION)
	);
	render_command_buffer.set_texture(mesh_texture_);
	render_command_buffer.draw_mesh(mesh_);

	SpriteBatch &sprite_batch = sprite_batch_;
	if (sprite_batch.begin_push()) {
		Font &font = font_;
		{
			Vector2 text_position(16.0f, HEIGHT - font.get_char_height() - 16.0f);
			push_text(sprite_batch, font, "Press space to jump", text_position, 2.0f, 1.0f);
		}
		sprite_batch.end_push();
		render_command_buffer.set_shader_pipeline(sprite_batch_shader_pipeline_);
		render_command_buffer.set_uniform_matrix4(sprite_batch_projection_uniform_, sprite_batch_projection_);
		render_command_buffer.set_texture(font.get_texture());
		render_command_buffer.draw_sprite_batch(sprite_batch);
	}
}

void Game::handle_key_event(const Key key, const bool down)
{
	switch (key) {
	case Key::LEFT_ARROW:
		left_ = down;
		break;
	case Key::RIGHT_ARROW:
		right_ = down;
		break;
	case Key::SPACE:
		jump_ = down;
		break;
	default:
		break;
	}
}

void Game::tick_position(const float delta_time)
{
	const bool left = left_;
	const bool right = right_;
	const float direction = (left && !right) ? -1.0f : (right && !left) ? 1.0f : 0.0f;
	velocity_.x = direction * MOVE_SPEED;

	if (jump_ && (mesh_position_.y <= 0.0f)) {
		velocity_.y = JUMP_SPEED;
	}
	velocity_.y += GRAVITY * delta_time;

	mesh_position_ += velocity_ * delta_time;
	if (mesh_position_.y < 0.0f) {
		mesh_position_.y = 0.0f;
	}
}
