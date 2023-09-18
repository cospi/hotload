#ifndef HOTLOAD_GAME_GAME_HH_
#define HOTLOAD_GAME_GAME_HH_

class Game final {
public:
	Game() = default;
	~Game() = default;

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	bool init();

	void tick();
};

#endif // HOTLOAD_GAME_GAME_HH_
