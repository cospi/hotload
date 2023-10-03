// \note Prevent ERROR from conflicting with LogLevel::ERROR.
#define NOGDI
#include <Windows.h>

#include <common/game/game_api.hh>

#include "../../log/stdlib/stdlib_logger.hh"
#include "../../shared_library/win32/win32_shared_library.hh"

#define SHARED_LIBRARY_FILENAME "libhotloadgame.so"

static bool load_game_api(Win32SharedLibrary &shared_library, GameApi &game_api, ILogger &logger)
{
	shared_library.fini();
	if (!shared_library.init(SHARED_LIBRARY_FILENAME)) {
		return false;
	}

	void (*populate_game_api)(void *);
	if (!shared_library.try_get_symbol("populate_game_api", reinterpret_cast<void **>(&populate_game_api))) {
		return false;
	}

	populate_game_api(&game_api);
	if (
		(game_api.game_memory_size == 0)
		|| (game_api.render_command_capacity == 0)
		|| (game_api.window_width == 0)
		|| (game_api.window_height == 0)
		|| (game_api.window_title == nullptr)
		|| (game_api.init == nullptr)
		|| (game_api.fini == nullptr)
		|| (game_api.handle_event == nullptr)
		|| (game_api.tick == nullptr)
	) {
		logger.log(LogLevel::ERROR, "Invalid game API specification.");
		return false;
	}

	return true;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int cmd_show)
{
	static_cast<void>(instance);
	static_cast<void>(previous_instance);
	static_cast<void>(cmd_line);
	static_cast<void>(cmd_show);

	StdlibLogger logger(stdout /* Info */, stderr /* Warning */, stderr /* Error */);

	Win32SharedLibrary shared_library(logger);
	GameApi game_api;
	if (!load_game_api(shared_library, game_api, logger)) {
		return -1;
	}

	return 0;
}
