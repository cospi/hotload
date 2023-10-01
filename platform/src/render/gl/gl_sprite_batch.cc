#include "gl_sprite_batch.hh"

#include <cassert>
#include <cstdint>
#include <limits>

#include <common/memory/allocation.hh>

static const std::size_t VERTICES_PER_SPRITE = 4;
static const std::size_t INDICES_PER_SPRITE = 6;

struct SpriteVertex {
	SpriteVertex() = default;

	SpriteVertex(const Vector2 &in_position, const Vector2 &in_uv)
		: position(in_position)
		, uv(in_uv)
	{ }

	Vector2 position;
	Vector2 uv;
};

GlSpriteBatch::GlSpriteBatch(ILogger &logger, IAllocator &allocator)
	: logger_(logger)
	, allocator_(allocator)
	, vertex_array_(logger)
	, vertex_buffer_(logger)
	, index_buffer_(logger)
	, sprite_capacity_(0)
	, sprite_count_(0)
	, mapped_buffer_(nullptr)
{ }

bool GlSpriteBatch::init(const std::size_t sprite_capacity)
{
	assert(sprite_capacity > 0);
	assert(sprite_capacity < (std::numeric_limits<std::uint16_t>::max() / INDICES_PER_SPRITE));

	{
		Allocation indices_allocation(allocator_);
		const std::size_t indices_size = sprite_capacity * INDICES_PER_SPRITE * sizeof(std::uint16_t);
		if (!indices_allocation.init(indices_size)) {
			return false;
		}

		std::uint16_t *const indices = static_cast<std::uint16_t *>(indices_allocation.get_memory());
		for (std::size_t i = 0; i < sprite_capacity; ++i) {
			const uint16_t vertex_offset = static_cast<std::uint16_t>(i * VERTICES_PER_SPRITE);
			const std::size_t indices_offset = i * INDICES_PER_SPRITE;
			indices[indices_offset] = vertex_offset;
			indices[indices_offset + 1] = static_cast<std::uint16_t>(vertex_offset + 1);
			indices[indices_offset + 2] = static_cast<std::uint16_t>(vertex_offset + 2);
			indices[indices_offset + 3] = vertex_offset;
			indices[indices_offset + 4] = static_cast<std::uint16_t>(vertex_offset + 2);
			indices[indices_offset + 5] = static_cast<std::uint16_t>(vertex_offset + 3);
		}

		const GLuint vertex_array = vertex_array_.vertex_array;
		glBindVertexArray(vertex_array);

		const GLuint vertex_buffer = vertex_buffer_.buffer;
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(
			GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(sprite_capacity * VERTICES_PER_SPRITE * sizeof(SpriteVertex)),
			nullptr,
			GL_DYNAMIC_DRAW
		);

		const GLuint index_buffer = index_buffer_.buffer;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices_size), indices, GL_STATIC_DRAW);
		// \note After glBufferData, indices memory can be freed. Hence the allocation scope.
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(SpriteVertex),
		static_cast<void *>(offsetof(SpriteVertex, position))
	);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(SpriteVertex),
		reinterpret_cast<void *>(offsetof(SpriteVertex, uv))
	);

	sprite_capacity_ = sprite_capacity;
	return true;
}

bool GlSpriteBatch::begin_push()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_.buffer);
	void *const buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (buffer == nullptr) {
		logger_.log(LogLevel::ERROR, "Mapping buffer failed.");
		return false;
	}

	mapped_buffer_ = buffer;
	sprite_count_ = 0;
	return true;
}

void GlSpriteBatch::push_sprite(const Sprite &sprite)
{
	assert(mapped_buffer_ != nullptr);
	assert(sprite_count_ < sprite_capacity_);

	const Rect &rect = sprite.rect;
	const Vector2 rect_min = rect.get_min();
	const Vector2 rect_max = rect.get_max();

	const Rect &uv = sprite.uv;
	const Vector2 uv_min = uv.get_min();
	const Vector2 uv_max = uv.get_max();

	SpriteVertex *const vertex_memory =
		static_cast<SpriteVertex *>(mapped_buffer_) + (sprite_count_ * VERTICES_PER_SPRITE);
	vertex_memory[0] = SpriteVertex(rect_min, uv_min);
	vertex_memory[1] = SpriteVertex(Vector2(rect_max.x, rect_min.y), Vector2(uv_max.x, uv_min.y));
	vertex_memory[2] = SpriteVertex(rect_max, uv_max);
	vertex_memory[3] = SpriteVertex(Vector2(rect_min.x, rect_max.y), Vector2(uv_min.x, uv_max.y));
	++sprite_count_;
}

void GlSpriteBatch::end_push()
{
	assert(mapped_buffer_ != nullptr);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	mapped_buffer_ = nullptr;
}

const GlVertexArray &GlSpriteBatch::get_vertex_array() const
{
	return vertex_array_;
}

std::size_t GlSpriteBatch::get_index_count() const
{
	return sprite_count_ * INDICES_PER_SPRITE;
}
