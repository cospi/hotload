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
