#include <cstdio>

#include "../../shared_library/posix/posix_shared_library.hh"

int main()
{
	PosixSharedLibrary shared_library;
	if (!shared_library.init("libhotloadgame.so", RTLD_NOW)) {
		return -1;
	}

	void (*game_tick)();
	if (!shared_library.try_get_symbol("game_tick", reinterpret_cast<void **>(&game_tick))) {
		return -1;
	}
	if (game_tick == nullptr) {
		std::fputs("game_tick symbol was null.\n", stderr);
		return -1;
	}

	game_tick();

	return 0;
}
