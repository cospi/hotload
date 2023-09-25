#ifndef HOTLOAD_GAME_GAME_HH_
#define HOTLOAD_GAME_GAME_HH_

#include <common/events/event.hh>
#include <common/platform/platform.hh>

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
	ShaderPipeline shader_pipeline_;
	std::int32_t projection_uniform_;
	std::int32_t translation_uniform_;
	Texture texture_;
	Mesh mesh_;
	Matrix4 projection_;
	Vector3 position_;
	bool up_;
	bool down_;
	bool left_;
	bool right_;
};

#endif // HOTLOAD_GAME_GAME_HH_
