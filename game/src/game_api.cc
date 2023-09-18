#include "game_api.hh"

#include <cassert>
#include <cinttypes>
#include <new>

#include <common/game/game_api.hh>

#include "game.hh"

static bool init(void *const game_memory, void *const platform)
{
	assert(game_memory != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(game_memory) % alignof(Game)) == 0);
	assert(platform != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(platform) % alignof(Platform)) == 0);

	Game *const game = new(game_memory) Game(*static_cast<Platform *>(platform));
	if (!game->init()) {
		game->~Game();
		return false;
	}
	return true;
}

static void fini(void *const game_memory)
{
	assert(game_memory != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(game_memory) % alignof(Game)) == 0);

	static_cast<Game *>(game_memory)->~Game();
}

static void tick(void *const game_memory)
{
	assert(game_memory != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(game_memory) % alignof(Game)) == 0);

	static_cast<Game *>(game_memory)->tick();
}

void populate_game_api(void *const game_api)
{
	assert(game_api != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(game_api) % alignof(GameApi)) == 0);

	GameApi *const game_api_actual = static_cast<GameApi *>(game_api);
	game_api_actual->game_memory_size = sizeof(Game);
	game_api_actual->window_width = 640;
	game_api_actual->window_height = 480;
	game_api_actual->window_title = "Hotload";
	game_api_actual->init = init;
	game_api_actual->fini = fini;
	game_api_actual->tick = tick;
}
