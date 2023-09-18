#include "../../log/stdlib/stdlib_logger.hh"
#include "../../shared_library/posix/posix_shared_library.hh"

int main()
{
	StdlibLogger logger(stdout /* Info */, stderr /* Warning */, stderr /* Error */);

	PosixSharedLibrary shared_library(logger);
	if (!shared_library.init("libhotloadgame.so", RTLD_NOW)) {
		return -1;
	}

	void (*game_tick)();
	if (!shared_library.try_get_symbol("game_tick", reinterpret_cast<void **>(&game_tick))) {
		return -1;
	}
	if (game_tick == nullptr) {
		logger.log(LogLevel::ERROR, "game_tick symbol was null.");
		return -1;
	}

	game_tick();

	return 0;
}
