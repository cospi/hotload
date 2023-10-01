#ifndef HOTLOAD_GAME_GAME_HH_
#define HOTLOAD_GAME_GAME_HH_

#include <common/events/event.hh>
#include <common/platform/platform.hh>
#include <common/render/font.hh>

class Game final {
public:
	explicit Game(Platform &platform);
	~Game() = default;

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	bool init();

	void handle_event(const Event &event);
	void tick(float delta_time);

private:
	void handle_key_event(Key key, bool down);
	void tick_position(float delta_time);

	Platform &platform_;

	Font font_;

	SpriteBatch sprite_batch_;
	ShaderPipeline sprite_batch_shader_pipeline_;
	std::int32_t sprite_batch_projection_uniform_;
	Matrix4 sprite_batch_projection_;

	Mesh mesh_;
	ShaderPipeline mesh_shader_pipeline_;
	std::int32_t mesh_projection_uniform_;
	std::int32_t mesh_translation_uniform_;
	Texture mesh_texture_;
	Matrix4 mesh_projection_;
	Vector3 mesh_position_;

	Vector3 velocity_;

	bool left_;
	bool right_;
	bool jump_;
};

#endif // HOTLOAD_GAME_GAME_HH_
