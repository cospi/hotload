#ifndef HOTLOAD_COMMON_GAME_GAME_API_HH_
#define HOTLOAD_COMMON_GAME_GAME_API_HH_

#include <cstddef>

struct GameApi {
	std::size_t game_memory_size;
	std::size_t render_command_capacity;
	unsigned int window_width;
	unsigned int window_height;
	const char *window_title;
	bool (*init)(void *game_memory, void *game_api);
	void (*fini)(void *game_memory);
	void (*handle_event)(void *game_memory, void *event);
	void (*tick)(void *game_memory, float delta_time);
};

#endif // HOTLOAD_COMMON_GAME_GAME_API_HH_
