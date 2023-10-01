#include <common/render/render_command_buffer.hh>

#include <cassert>

RenderCommandBuffer::RenderCommandBuffer(IAllocator &allocator)
	: allocator_(allocator)
	, commands_(nullptr)
	, command_capacity_(0)
	, command_count_(0)
{ }

RenderCommandBuffer::~RenderCommandBuffer()
{
	if (RenderCommand *const commands = commands_; commands != nullptr) {
		allocator_.free(commands);
	}
}

bool RenderCommandBuffer::init(const std::size_t command_capacity)
{
	assert(commands_ == nullptr);
	assert(command_capacity > 0);

	void *const commands = allocator_.allocate(command_capacity * sizeof(RenderCommand));
	if (commands == nullptr) {
		return false;
	}

	commands_ = static_cast<RenderCommand *>(commands);
	command_capacity_ = command_capacity;
	command_count_ = 0;
	return true;
}

void RenderCommandBuffer::reset()
{
	assert(commands_ != nullptr);

	command_count_ = 0;
}

void RenderCommandBuffer::clear(const Color &color)
{
	RenderCommand command;
	command.type = RenderCommandType::CLEAR;
	command.command.clear.clear_color = color;
	push_command(command);
}

void RenderCommandBuffer::set_shader_pipeline(const ShaderPipeline &shader_pipeline)
{
	RenderCommand command;
	command.type = RenderCommandType::SET_SHADER_PIPELINE;
	command.command.set_shader_pipeline.shader_pipeline = shader_pipeline.get_shader_pipeline();
	push_command(command);
}

void RenderCommandBuffer::set_uniform_matrix4(std::int32_t uniform, const Matrix4 &matrix)
{
	RenderCommand command;
	command.type = RenderCommandType::SET_UNIFORM_MATRIX4;
	command.command.set_uniform_matrix4.uniform = uniform;
	command.command.set_uniform_matrix4.matrix = matrix;
	push_command(command);
}

void RenderCommandBuffer::set_texture(const Texture &texture)
{
	RenderCommand command;
	command.type = RenderCommandType::SET_TEXTURE;
	command.command.set_texture.texture = texture.get_texture();
	push_command(command);
}

void RenderCommandBuffer::draw_mesh(const Mesh &mesh)
{
	RenderCommand command;
	command.type = RenderCommandType::DRAW_MESH;
	command.command.draw_mesh.mesh = mesh.get_mesh();
	push_command(command);
}

void RenderCommandBuffer::draw_sprite_batch(const SpriteBatch &sprite_batch)
{
	RenderCommand command;
	command.type = RenderCommandType::DRAW_SPRITE_BATCH;
	command.command.draw_sprite_batch.sprite_batch = sprite_batch.get_sprite_batch();
	push_command(command);
}

const RenderCommand *RenderCommandBuffer::get_commands() const
{
	assert(commands_ != nullptr);

	return commands_;
}

std::size_t RenderCommandBuffer::get_command_capacity() const
{
	assert(commands_ != nullptr);

	return command_capacity_;
}

std::size_t RenderCommandBuffer::get_command_count() const
{
	assert(commands_ != nullptr);

	return command_count_;
}

void RenderCommandBuffer::push_command(const RenderCommand &command)
{
	assert(commands_ != nullptr);
	assert(command_count_ < command_capacity_);

	commands_[command_count_++] = command;
}
