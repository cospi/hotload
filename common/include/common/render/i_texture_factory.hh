#ifndef HOTLOAD_COMMON_RENDER_I_TEXTURE_FACTORY_HH_
#define HOTLOAD_COMMON_RENDER_I_TEXTURE_FACTORY_HH_

#include "image.hh"

class ITextureFactory {
public:
	virtual ~ITextureFactory() = default;

	virtual void *create_texture(const Image &image) = 0;
	virtual void destroy_texture(void *texture) = 0;
};

#endif // HOTLOAD_COMMON_RENDER_I_TEXTURE_FACTORY_HH_
