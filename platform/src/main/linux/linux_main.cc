#include <common/events/event.hh>
#include <common/game/game_api.hh>
#include <common/memory/allocation.hh>
#include <common/platform/platform.hh>

#include "../../file_system/posix/posix_file_system.hh"
#include "../../log/stdlib/stdlib_logger.hh"
#include "../../memory/stdlib/stdlib_allocator.hh"
#include "../../render/gl/gl_mesh_factory.hh"
#include "../../render/gl/gl_render.hh"
#include "../../render/gl/gl_shader_pipeline_factory.hh"
#include "../../render/gl/gl_sprite_batch_factory.hh"
#include "../../render/gl/gl_texture_factory.hh"
#include "../../shared_library/posix/posix_shared_library.hh"
#include "../../time/posix/posix_time.hh"
#include "../../window_system/x11/x11_gl_context.hh"
#include "../../window_system/x11/x11_gl_extensions.hh"
#include "../../window_system/x11/x11_keys.hh"

static void handle_key_press(const GameApi &game_api, void *const game_memory, XKeyEvent &key_event)
{
	Event event;
	event.type = EventType::KEY_DOWN;
	event.event.key_down.key = x11_get_key_event_key(key_event);
	game_api.handle_event(game_memory, &event);
}

static void handle_key_release(const GameApi &game_api, void *const game_memory, XKeyEvent &key_event)
{
	Event event;
	event.type = EventType::KEY_UP;
	event.event.key_up.key = x11_get_key_event_key(key_event);
	game_api.handle_event(game_memory, &event);
}

static bool handle_events(const X11Connection &x11_connection, const GameApi &game_api, void *const game_memory)
{
	const Atom wm_delete_window = x11_connection.get_wm_delete_window();
	Display *const display = x11_connection.get_display();
	while (XPending(display) > 0) {
		XEvent event;
		XNextEvent(display, &event);
		switch (event.type) {
		case ClientMessage:
			if (static_cast<Atom>(event.xclient.data.l[0]) == wm_delete_window) {
				return false;
			}
			break;
		case KeyPress:
			handle_key_press(game_api, game_memory, event.xkey);
			break;
		case KeyRelease:
			handle_key_release(game_api, game_memory, event.xkey);
			break;
		default:
			break;
		}
	}
	return true;
}

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
		return -1;
	}

	StdlibAllocator allocator(logger);
	Allocation game_memory_allocation(allocator);
	if (!game_memory_allocation.init(game_api.game_memory_size)) {
		return -1;
	}

	RenderCommandBuffer render_command_buffer(allocator);
	if (!render_command_buffer.init(game_api.render_command_capacity)) {
		return -1;
	}

	X11Connection x11_connection(logger);
	if (!x11_connection.init(nullptr)) {
		return -1;
	}

	X11GlWindow window(x11_connection, logger);
	if (!window.init(game_api.window_width, game_api.window_height, game_api.window_title)) {
		return -1;
	}

	X11GlContext gl_context(window, logger);
	if (!gl_context.init()) {
		return -1;
	}

	window.make_context_current(&gl_context);

	if (!x11_gl_init_extensions()) {
		logger.log(LogLevel::ERROR, "Initializing OpenGL extensions failed.");
		return -1;
	}

	PosixFileSystem file_system(logger);
	GlShaderPipelineFactory shader_pipeline_factory(logger, allocator, file_system);
	GlTextureFactory texture_factory(logger, allocator);
	GlMeshFactory mesh_factory(logger, allocator);
	GlSpriteBatchFactory sprite_batch_factory(logger, allocator);
	Platform platform(
		logger,
		allocator,
		file_system,
		shader_pipeline_factory,
		texture_factory,
		mesh_factory,
		sprite_batch_factory,
		render_command_buffer
	);
	void *const game_memory = game_memory_allocation.get_memory();

	if (!game_api.init(game_memory, &platform)) {
		logger.log(LogLevel::ERROR, "Game initialization failed.");
		return -1;
	}

	long long previous_time_nsec = posix_time_nsec();

	while (handle_events(x11_connection, game_api, game_memory)) {
		render_command_buffer.reset();

		if (const long long time_nsec = posix_time_nsec(); time_nsec > 0LL) {
			const float delta_time =  static_cast<float>(
				static_cast<long double>(time_nsec - previous_time_nsec) / NSEC_PER_SEC
			);
			game_api.tick(game_memory, delta_time);
			previous_time_nsec = time_nsec;
		}

		gl_render(render_command_buffer);
		window.swap_buffers();
	}

	game_api.fini(game_memory);

	window.make_context_current(nullptr);

	return 0;
}
