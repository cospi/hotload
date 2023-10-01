#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_SPRITE_BATCH_FACTORY_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_SPRITE_BATCH_FACTORY_HH_

#include <common/log/i_logger.hh>
#include <common/memory/i_allocator.hh>
#include <common/render/i_sprite_batch_factory.hh>

class GlSpriteBatchFactory final : public ISpriteBatchFactory {
public:
	GlSpriteBatchFactory(ILogger &logger, IAllocator &allocator);
	~GlSpriteBatchFactory() override = default;

	GlSpriteBatchFactory(const GlSpriteBatchFactory &) = delete;
	GlSpriteBatchFactory &operator=(const GlSpriteBatchFactory &) = delete;

	void *create_sprite_batch(std::size_t sprite_capacity) override;
	void destroy_sprite_batch(void *sprite_batch) override;
	bool begin_push(void *sprite_batch) override;
	void push_sprite(void *sprite_batch, const Sprite &sprite) override;
	void end_push(void *sprite_batch) override;

private:
	ILogger &logger_;
	IAllocator &allocator_;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_SPRITE_BATCH_FACTORY_HH_
