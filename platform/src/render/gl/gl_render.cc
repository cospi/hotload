#include "gl_render.hh"

#include <GL/gl.h>

#include <cassert>
#include <cinttypes>

#include <common/render/render_buffer.hh>

#include "gl_mesh.hh"
#include "gl_program.hh"
#include "gl_sprite_batch.hh"
#include "gl_texture.hh"

static GLbitfield convert_clear_buffer_flags(const int buffer_flags)
{
	GLbitfield flags = 0;
	if ((buffer_flags & RenderBuffer::COLOR) != 0) {
		flags |= GL_COLOR_BUFFER_BIT;
	}
	if ((buffer_flags & RenderBuffer::DEPTH) != 0) {
		flags |= GL_DEPTH_BUFFER_BIT;
	}
	return flags;
}

static void render_clear_render_target(const RenderCommandClearRenderTarget &clear_render_target)
{
	const int buffer_flags = clear_render_target.buffer_flags;
	if ((buffer_flags & RenderBuffer::COLOR) != 0) {
		const Color &color = clear_render_target.color;
		glClearColor(color.r, color.g, color.b, color.a);
	}
	glClear(convert_clear_buffer_flags(buffer_flags));
}

static void render_set_shader_pipeline(const RenderCommandSetShaderPipeline &set_shader_pipeline)
{
	assert(set_shader_pipeline.shader_pipeline != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(set_shader_pipeline.shader_pipeline) % alignof(GlProgram)) == 0);

	glUseProgram(static_cast<const GlProgram *>(set_shader_pipeline.shader_pipeline)->get_program());
}

static void render_set_uniform_matrix4(const RenderCommandSetUniformMatrix4 &set_uniform_matrix4)
{
	glUniformMatrix4fv(set_uniform_matrix4.uniform, 1, GL_TRUE, set_uniform_matrix4.matrix.elements);
}

static void render_set_texture(const RenderCommandSetTexture &set_texture)
{
	assert(set_texture.texture != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(set_texture.texture) % alignof(GlTexture)) == 0);

	glBindTexture(GL_TEXTURE_2D, static_cast<const GlTexture *>(set_texture.texture)->texture);
}

static void render_draw_mesh(const RenderCommandDrawMesh &draw_mesh)
{
	assert(draw_mesh.mesh != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(draw_mesh.mesh) % alignof(GlMesh)) == 0);

	const GlMesh &mesh = *static_cast<const GlMesh *>(draw_mesh.mesh);
	glBindVertexArray(mesh.get_vertex_array().vertex_array);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.index_count), GL_UNSIGNED_SHORT, nullptr);
}

static void render_draw_sprite_batch(const RenderCommandDrawSpriteBatch &draw_sprite_batch)
{
	assert(draw_sprite_batch.sprite_batch != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(draw_sprite_batch.sprite_batch) % alignof(GlSpriteBatch)) == 0);

	const GlSpriteBatch &sprite_batch = *static_cast<const GlSpriteBatch *>(draw_sprite_batch.sprite_batch);
	glBindVertexArray(sprite_batch.get_vertex_array().vertex_array);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sprite_batch.get_index_count()), GL_UNSIGNED_SHORT, nullptr);
}

static void render(const RenderCommand &command)
{
	switch (command.type) {
	case RenderCommandType::CLEAR_RENDER_TARGET:
		render_clear_render_target(command.command.clear_render_target);
		break;
	case RenderCommandType::SET_SHADER_PIPELINE:
		render_set_shader_pipeline(command.command.set_shader_pipeline);
		break;
	case RenderCommandType::SET_UNIFORM_MATRIX4:
		render_set_uniform_matrix4(command.command.set_uniform_matrix4);
		break;
	case RenderCommandType::SET_TEXTURE:
		render_set_texture(command.command.set_texture);
		break;
	case RenderCommandType::DRAW_MESH:
		render_draw_mesh(command.command.draw_mesh);
		break;
	case RenderCommandType::DRAW_SPRITE_BATCH:
		render_draw_sprite_batch(command.command.draw_sprite_batch);
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
