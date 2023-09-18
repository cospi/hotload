#include "gl_render.hh"

#include <GL/gl.h>

#include <cassert>

static void render_clear(const RenderCommandClear &clear)
{
	const Color &color = clear.clear_color;
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

static void render(const RenderCommand &command)
{
	switch (command.type) {
	case RenderCommandType::CLEAR:
		render_clear(command.command.clear);
		break;
	default:
		assert(false);
		break;
	}
}

void gl_render(const RenderCommandBuffer &render_command_buffer)
{
	const RenderCommand *const commands = render_command_buffer.get_commands();
	const std::size_t count = render_command_buffer.get_command_count();
	for (std::size_t i = 0; i < count; ++i) {
		render(commands[i]);
	}
}
