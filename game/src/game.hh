#ifndef HOTLOAD_GAME_GAME_HH_
#define HOTLOAD_GAME_GAME_HH_

#include <common/platform/platform.hh>

class Game final {
public:
	explicit Game(Platform &platform);
	~Game() = default;

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	bool init();

	void tick();

private:
	Platform &platform_;
	ShaderPipeline shader_pipeline_;
	std::int32_t projection_uniform_;
	Texture texture_;
	Mesh mesh_;
	Matrix4 projection_;
};

#endif // HOTLOAD_GAME_GAME_HH_
