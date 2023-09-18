#include <common/game/game_api.hh>
#include <common/memory/allocation.hh>
#include <common/platform/platform.hh>

#include "../../file_system/posix/posix_file_system.hh"
#include "../../log/stdlib/stdlib_logger.hh"
#include "../../memory/stdlib/stdlib_allocator.hh"
#include "../../shared_library/posix/posix_shared_library.hh"

int main()
{
	StdlibLogger logger(stdout /* Info */, stderr /* Warning */, stderr /* Error */);

	PosixSharedLibrary shared_library(logger);
	if (!shared_library.init("libhotloadgame.so", RTLD_NOW)) {
		return -1;
	}

	void (*populate_game_api)(void *);
	if (!shared_library.try_get_symbol("populate_game_api", reinterpret_cast<void **>(&populate_game_api))) {
		return -1;
	}
	if (populate_game_api == nullptr) {
		logger.log(LogLevel::ERROR, "populate_game_api symbol was null.");
		return -1;
	}

	GameApi game_api;
	populate_game_api(&game_api);
	if (
		(game_api.game_memory_size == 0)
		|| (game_api.init == nullptr)
		|| (game_api.fini == nullptr)
		|| (game_api.tick == nullptr)
	) {
		logger.log(LogLevel::ERROR, "Invalid game API specification.");
		return -1;
	}

	StdlibAllocator allocator(logger);
	Allocation game_memory_allocation(allocator);
	if (!game_memory_allocation.init(game_api.game_memory_size)) {
		return -1;
	}
	void *const game_memory = game_memory_allocation.get_memory();

	PosixFileSystem file_system(logger);
	Platform platform(logger, allocator, file_system);

	if (!game_api.init(game_memory, &platform)) {
		logger.log(LogLevel::ERROR, "Game initialization failed.");
		return -1;
	}

	game_api.tick(game_memory);
	game_api.fini(game_memory);

	return 0;
}
