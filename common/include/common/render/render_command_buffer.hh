#ifndef HOTLOAD_COMMON_RENDER_RENDER_COMMAND_BUFFER_HH_
#define HOTLOAD_COMMON_RENDER_RENDER_COMMAND_BUFFER_HH_

#include "color.hh"
#include "../memory/i_allocator.hh"

enum class RenderCommandType {
	CLEAR
};

struct RenderCommandClear {
	Color clear_color;
};

struct RenderCommand {
	RenderCommandType type;
	union {
		RenderCommandClear clear;
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
