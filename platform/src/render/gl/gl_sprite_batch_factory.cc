#include "gl_sprite_batch_factory.hh"

#include <cassert>
#include <cinttypes>
#include <new>

#include "gl_sprite_batch.hh"

GlSpriteBatchFactory::GlSpriteBatchFactory(ILogger &logger, IAllocator &allocator)
	: logger_(logger)
	, allocator_(allocator)
{ }

void *GlSpriteBatchFactory::create_sprite_batch(const std::size_t sprite_capacity)
{
	assert(sprite_capacity > 0);

	IAllocator &allocator = allocator_;
	void *const sprite_batch_memory = allocator.allocate(sizeof(GlSpriteBatch));
	if (sprite_batch_memory == nullptr) {
		return nullptr;
	}

	GlSpriteBatch *const sprite_batch = new(sprite_batch_memory) GlSpriteBatch(logger_, allocator);
	if (!sprite_batch->init(sprite_capacity)) {
		allocator.free(sprite_batch_memory);
		return nullptr;
	}

	return sprite_batch_memory;
}

void GlSpriteBatchFactory::destroy_sprite_batch(void *const sprite_batch)
{
	assert(sprite_batch != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(sprite_batch) % alignof(GlSpriteBatch)) == 0);

	static_cast<GlSpriteBatch *>(sprite_batch)->~GlSpriteBatch();
	allocator_.free(sprite_batch);
}

bool GlSpriteBatchFactory::begin_push(void *const sprite_batch)
{
	assert(sprite_batch != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(sprite_batch) % alignof(GlSpriteBatch)) == 0);

	return static_cast<GlSpriteBatch *>(sprite_batch)->begin_push();
}

void GlSpriteBatchFactory::push_sprite(void *const sprite_batch, const Sprite &sprite)
{
	assert(sprite_batch != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(sprite_batch) % alignof(GlSpriteBatch)) == 0);

	static_cast<GlSpriteBatch *>(sprite_batch)->push_sprite(sprite);
}

void GlSpriteBatchFactory::end_push(void *const sprite_batch)
{
	assert(sprite_batch != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(sprite_batch) % alignof(GlSpriteBatch)) == 0);

	static_cast<GlSpriteBatch *>(sprite_batch)->end_push();
}
