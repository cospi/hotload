#ifndef HOTLOAD_COMMON_RENDER_RENDER_COMMAND_BUFFER_HH_
#define HOTLOAD_COMMON_RENDER_RENDER_COMMAND_BUFFER_HH_

#include <cstdint>

#include "color.hh"
#include "mesh.hh"
#include "shader_pipeline.hh"
#include "texture.hh"
#include "../math/matrix4.hh"
#include "../memory/i_allocator.hh"

enum class RenderCommandType {
	CLEAR,
	SET_SHADER_PIPELINE,
	SET_UNIFORM_MATRIX4,
	SET_TEXTURE,
	DRAW_MESH
};

struct RenderCommandClear {
	RenderCommandClear() = default;

	explicit RenderCommandClear(const Color &in_clear_color)
		: clear_color(in_clear_color)
	{ }

	Color clear_color;
};

struct RenderCommandSetShaderPipeline {
	RenderCommandSetShaderPipeline() = default;

	explicit RenderCommandSetShaderPipeline(void *const in_shader_pipeline)
		: shader_pipeline(in_shader_pipeline)
	{ }

	void *shader_pipeline;
};

struct RenderCommandSetUniformMatrix4 {
	RenderCommandSetUniformMatrix4() = default;

	explicit RenderCommandSetUniformMatrix4(const std::int32_t in_uniform, const Matrix4 &in_matrix)
		: uniform(in_uniform)
		, matrix(in_matrix)
	{ }

	std::int32_t uniform;
	Matrix4 matrix;
};

struct RenderCommandSetTexture {
	RenderCommandSetTexture() = default;

	explicit RenderCommandSetTexture(void *const in_texture)
		: texture(in_texture)
	{ }

	void *texture;
};

struct RenderCommandDrawMesh {
	RenderCommandDrawMesh() = default;

	explicit RenderCommandDrawMesh(void *const in_mesh)
		: mesh(in_mesh)
	{ }

	void *mesh;
};

struct RenderCommand {
	RenderCommandType type;
	union {
		RenderCommandClear clear;
		RenderCommandSetShaderPipeline set_shader_pipeline;
		RenderCommandSetUniformMatrix4 set_uniform_matrix4;
		RenderCommandSetTexture set_texture;
		RenderCommandDrawMesh draw_mesh;
	} command;
};

class RenderCommandBuffer final {
public:
	explicit RenderCommandBuffer(IAllocator &allocator);
	~RenderCommandBuffer();

	RenderCommandBuffer(const RenderCommandBuffer &) = delete;
	RenderCommandBuffer &operator=(const RenderCommandBuffer &) = delete;

	bool init(std::size_t command_capacity);

	void reset();

	void clear(const Color &color);
	void set_shader_pipeline(const ShaderPipeline &shader_pipeline);
	void set_uniform_matrix4(std::int32_t uniform, const Matrix4 &matrix);
	void set_texture(const Texture &texture);
	void draw_mesh(const Mesh &mesh);

	const RenderCommand *get_commands() const;
	std::size_t get_command_capacity() const;
	std::size_t get_command_count() const;

private:
	void push_command(const RenderCommand &command);

	IAllocator &allocator_;
	RenderCommand *commands_;
	std::size_t command_capacity_;
	std::size_t command_count_;
};

#endif // HOTLOAD_COMMON_RENDER_RENDER_COMMAND_BUFFER_HH_
