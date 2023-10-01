#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_TEXTURE_FACTORY_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_TEXTURE_FACTORY_HH_

#include <common/log/i_logger.hh>
#include <common/memory/i_allocator.hh>
#include <common/render/i_texture_factory.hh>

class GlTextureFactory final : public ITextureFactory {
public:
	explicit GlTextureFactory(ILogger &logger, IAllocator &allocator);
	~GlTextureFactory() override = default;

	void *create_texture(const Image &image, TextureFilter filter) override;
	void destroy_texture(void *texture) override;

private:
	ILogger &logger_;
	IAllocator &allocator_;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_TEXTURE_FACTORY_HH_
