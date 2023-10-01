#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_SPRITE_BATCH_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_SPRITE_BATCH_HH_

#include <common/memory/i_allocator.hh>
#include <common/render/sprite.hh>

#include "gl_buffer.hh"
#include "gl_vertex_array.hh"

class GlSpriteBatch final {
public:
	GlSpriteBatch(ILogger &logger, IAllocator &allocator);
	~GlSpriteBatch() = default;

	GlSpriteBatch(const GlSpriteBatch &) = delete;
	GlSpriteBatch &operator=(const GlSpriteBatch &) = delete;

	bool init(std::size_t sprite_capacity);

	bool begin_push();
	void push_sprite(const Sprite &sprite);
	void end_push();

	const GlVertexArray &get_vertex_array() const;
	std::size_t get_index_count() const;

private:
	ILogger &logger_;
	IAllocator &allocator_;
	GlVertexArray vertex_array_;
	GlBuffer vertex_buffer_;
	GlBuffer index_buffer_;
	std::size_t sprite_capacity_;
	std::size_t sprite_count_;
	void *mapped_buffer_;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_SPRITE_BATCH_HH_
