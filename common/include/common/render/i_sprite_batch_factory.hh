#ifndef HOTLOAD_COMMON_RENDER_I_SPRITE_BATCH_FACTORY_HH_
#define HOTLOAD_COMMON_RENDER_I_SPRITE_BATCH_FACTORY_HH_

#include <cstddef>

#include "sprite.hh"

class ISpriteBatchFactory {
public:
	virtual ~ISpriteBatchFactory() = default;

	virtual void *create_sprite_batch(std::size_t sprite_capacity) = 0;
	virtual void destroy_sprite_batch(void *sprite_batch) = 0;
	virtual bool begin_push(void *sprite_batch) = 0;
	virtual void push_sprite(void *sprite_batch, const Sprite &sprite) = 0;
	virtual void end_push(void *sprite_batch) = 0;
};

#endif // HOTLOAD_COMMON_RENDER_I_SPRITE_BATCH_FACTORY_HH_
