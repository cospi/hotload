#include <Windows.h>

#include <common/events/event.hh>
#include <common/game/game_api.hh>
#include <common/memory/allocation.hh>
#include <common/platform/platform.hh>

#include "../../file_system/win32/win32_file_system.hh"
#include "../../log/stdlib/stdlib_logger.hh"
#include "../../memory/stdlib/stdlib_allocator.hh"
#include "../../render/gl/gl.hh"
#include "../../render/gl/gl_mesh_factory.hh"
#include "../../render/gl/gl_render.hh"
#include "../../render/gl/gl_shader_pipeline_factory.hh"
#include "../../render/gl/gl_sprite_batch_factory.hh"
#include "../../render/gl/gl_texture_factory.hh"
#include "../../shared_library/win32/win32_shared_library.hh"
#include "../../time/win32/win32_time.hh"
#include "../../window_system/win32/win32_gl_context.hh"
#include "../../window_system/win32/win32_gl_extensions.hh"
#include "../../window_system/win32/win32_gl_window.hh"
#include "../../window_system/win32/win32_keys.hh"

#define SHARED_LIBRARY_FILENAME "libhotloadgame.so"
#define SHARED_LIBRARY_PATH (BIN_DIR "/" SHARED_LIBRARY_FILENAME)

static const wchar_t WINDOW_TITLE[] = L"Hotload";
static const wchar_t WINDOW_CLASS_NAME[] = L"HotloadWindowClass";

static const long double SHARED_LIBRARY_CHECK_INTERVAL_SEC = 1.0L;

static bool s_running = true;
static void *s_game_memory = nullptr;
static GameApi s_game_api;

static bool load_game_api(Win32SharedLibrary &shared_library, ILogger &logger)
{
	shared_library.fini();
	if (!shared_library.init(SHARED_LIBRARY_FILENAME)) {
		return false;
	}

	void (*populate_game_api)(void *);
	if (!shared_library.try_get_symbol("populate_game_api", reinterpret_cast<void **>(&populate_game_api))) {
		return false;
	}

	populate_game_api(&s_game_api);
	if (
		(s_game_api.game_memory_size == 0)
		|| (s_game_api.render_command_capacity == 0)
		|| (s_game_api.window_width == 0)
		|| (s_game_api.window_height == 0)
		|| (s_game_api.window_title == nullptr)
		|| (s_game_api.init == nullptr)
		|| (s_game_api.fini == nullptr)
		|| (s_game_api.handle_event == nullptr)
		|| (s_game_api.tick == nullptr)
	) {
		logger.log(LogLevel::ERR, "Invalid game API specification.");
		return false;
	}

	return true;
}

static void handle_key_down(const WPARAM win32_key)
{
	Event event;
	event.type = EventType::KEY_DOWN;
	event.event.key_down.key = win32_get_key(win32_key);
	s_game_api.handle_event(s_game_memory, &event);
}

static void handle_key_up(const WPARAM win32_key)
{
	Event event;
	event.type = EventType::KEY_UP;
	event.event.key_down.key = win32_get_key(win32_key);
	s_game_api.handle_event(s_game_memory, &event);
}

static bool handle_events()
{
	for (MSG message; PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE) != 0;) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
	return s_running;
}

static LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	switch (message) {
	case WM_CLOSE:
		s_running = false;
		return 0;
	case WM_KEYDOWN:
		handle_key_down(w_param);
		break;
	case WM_KEYUP:
		handle_key_up(w_param);
		break;
	default:
		break;
	}

	return DefWindowProcW(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int cmd_show)
{
	static_cast<void>(instance);
	static_cast<void>(previous_instance);
	static_cast<void>(cmd_line);
	static_cast<void>(cmd_show);

	StdlibLogger logger(stdout /* Info */, stderr /* Warning */, stderr /* Error */);

	Win32SharedLibrary shared_library(logger);
	if (!load_game_api(shared_library, logger)) {
		return -1;
	}

	StdlibAllocator allocator(logger);
	Allocation game_memory_allocation(allocator);
	if (!game_memory_allocation.init(s_game_api.game_memory_size)) {
		return -1;
	}

	RenderCommandBuffer render_command_buffer(allocator);
	if (!render_command_buffer.init(s_game_api.render_command_capacity)) {
		return -1;
	}

	if (!win32_gl_init_context_creation_extensions(instance, logger)) {
		logger.log(LogLevel::ERR, "Initializing OpenGL context creation extensions failed.");
		return -1;
	}

	Win32GlWindow window(logger);
	if (!window.init(
		s_game_api.window_width,
		s_game_api.window_height,
		WINDOW_TITLE,
		CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
		window_proc,
		instance,
		LoadCursor(nullptr, IDC_ARROW),
		WINDOW_CLASS_NAME
	)) {
		return -1;
	}

	const HDC device_context = window.get_device_context();
	Win32GlContext gl_context(logger);
	if (!gl_context.init(device_context)) {
		return -1;
	}

	if (wglMakeCurrent(device_context, gl_context.get_context()) == FALSE) {
		logger.log(LogLevel::ERR, "Making context current failed.");
		return -1;
	}

	if (!win32_gl_init_extensions()) {
		logger.log(LogLevel::ERR, "Initializing OpenGL extensions failed.");
		return -1;
	}

	window.show();

	Win32FileSystem file_system(logger);
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
	s_game_memory = game_memory_allocation.get_memory();

	if (!s_game_api.init(s_game_memory, &platform)) {
		logger.log(LogLevel::ERR, "Game initialization failed.");
		return -1;
	}

	std::time_t previous_shared_library_modification_time;
	if (!file_system.try_get_file_last_modification_time(
		SHARED_LIBRARY_PATH,
		&previous_shared_library_modification_time
	)) {
		return -1;
	}

	const LONGLONG performance_frequency = win32_performance_frequency();
	if (performance_frequency == -1L) {
		logger.log(LogLevel::ERR, "Getting performance frequency failed.");
		return -1;
	}

	LONGLONG previous_performance_counter = win32_performance_counter();
	if (previous_performance_counter == -1LL) {
		logger.log(LogLevel::ERR, "Getting initial performance counter failed.");
		return -1;
	}
	LONGLONG previous_shared_library_check_performance_counter = previous_performance_counter;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (handle_events()) {
		const LONGLONG performance_counter = win32_performance_counter();
		if (performance_counter == -1LL) {
			continue;
		}

		if (
			(static_cast<long double>(
				performance_counter - previous_shared_library_check_performance_counter
			) / performance_frequency)
			>= SHARED_LIBRARY_CHECK_INTERVAL_SEC
		) {
			std::time_t shared_library_modification_time;
			if (
				file_system.try_get_file_last_modification_time(
					SHARED_LIBRARY_PATH,
					&shared_library_modification_time
				)
				&& (shared_library_modification_time > previous_shared_library_modification_time)
			) {
				if (!load_game_api(shared_library, logger)) {
					break;
				}
				previous_shared_library_modification_time = shared_library_modification_time;
			}
			previous_shared_library_check_performance_counter = performance_counter;
		}

		render_command_buffer.reset();

		const long double delta_time = static_cast<long double>(
			performance_counter - previous_performance_counter
		) / performance_frequency;
		s_game_api.tick(s_game_memory, static_cast<float>(delta_time));
		previous_performance_counter = performance_counter;

		gl_render(render_command_buffer);
		SwapBuffers(device_context);
	}

	s_game_api.fini(s_game_memory);

	wglMakeCurrent(nullptr, nullptr);

	return 0;
}
