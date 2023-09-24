#include <GL/gl.h>

#include <common/game/game_api.hh>
#include <common/memory/allocation.hh>
#include <common/platform/platform.hh>

#include "../../file_system/posix/posix_file_system.hh"
#include "../../log/stdlib/stdlib_logger.hh"
#include "../../memory/stdlib/stdlib_allocator.hh"
#include "../../render/gl/gl_mesh_factory.hh"
#include "../../render/gl/gl_render.hh"
#include "../../render/gl/gl_shader_pipeline_factory.hh"
#include "../../render/gl/gl_texture_factory.hh"
#include "../../shared_library/posix/posix_shared_library.hh"
#include "../../window_system/x11/x11_gl_context.hh"
#include "../../window_system/x11/x11_gl_extensions.hh"

static bool handle_events(const X11Connection &x11_connection)
{
	const Atom wm_delete_window = x11_connection.get_wm_delete_window();
	Display *const display = x11_connection.get_display();
	while (XPending(display) > 0) {
		XEvent e;
		XNextEvent(display, &e);
		if ((e.type == ClientMessage) && (static_cast<Atom>(e.xclient.data.l[0]) == wm_delete_window)) {
			return false;
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
	Platform platform(
		logger,
		allocator,
		file_system,
		shader_pipeline_factory,
		texture_factory,
		mesh_factory,
		render_command_buffer
	);
	void *const game_memory = game_memory_allocation.get_memory();

	if (!game_api.init(game_memory, &platform)) {
		logger.log(LogLevel::ERROR, "Game initialization failed.");
		return -1;
	}

	while (handle_events(x11_connection)) {
		render_command_buffer.reset();
		game_api.tick(game_memory);
		gl_render(render_command_buffer);
		window.swap_buffers();
	}

	game_api.fini(game_memory);

	window.make_context_current(nullptr);

	return 0;
}
