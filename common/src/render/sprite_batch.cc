#include <common/render/sprite_batch.hh>

#include <cassert>

SpriteBatch::SpriteBatch(ISpriteBatchFactory &factory)
	: factory_(factory)
	, sprite_batch_(nullptr)
{ }

SpriteBatch::~SpriteBatch()
{
	if (void *const sprite_batch = sprite_batch_; sprite_batch != nullptr) {
		factory_.destroy_sprite_batch(sprite_batch);
	}
}

bool SpriteBatch::init(const std::size_t sprite_capacity)
{
	assert(sprite_batch_ == nullptr);
	assert(sprite_capacity > 0);

	void *const sprite_batch = factory_.create_sprite_batch(sprite_capacity);
	if (sprite_batch == nullptr) {
		return false;
	}

	sprite_batch_ = sprite_batch;
	return true;
}


bool SpriteBatch::begin_push()
{
	assert(sprite_batch_ != nullptr);

	return factory_.begin_push(sprite_batch_);
}

void SpriteBatch::push_sprite(const Sprite &sprite)
{
	assert(sprite_batch_ != nullptr);

	factory_.push_sprite(sprite_batch_, sprite);
}

void SpriteBatch::end_push()
{
	assert(sprite_batch_ != nullptr);

	factory_.end_push(sprite_batch_);
}

void *SpriteBatch::get_sprite_batch() const
{
	assert(sprite_batch_ != nullptr);

	return sprite_batch_;
}
