#ifndef HOTLOAD_COMMON_RENDER_SPRITE_BATCH_HH_
#define HOTLOAD_COMMON_RENDER_SPRITE_BATCH_HH_

#include "i_sprite_batch_factory.hh"

class SpriteBatch final {
public:
	explicit SpriteBatch(ISpriteBatchFactory &factory);
	~SpriteBatch();

	SpriteBatch(const SpriteBatch &) = delete;
	SpriteBatch &operator=(const SpriteBatch &) = delete;

	bool init(std::size_t sprite_capacity);

	bool begin_push();
	void push_sprite(const Sprite &sprite);
	void end_push();

	void *get_sprite_batch() const;

private:
	ISpriteBatchFactory &factory_;
	void *sprite_batch_;
};

#endif // HOTLOAD_COMMON_RENDER_SPRITE_BATCH_HH_
