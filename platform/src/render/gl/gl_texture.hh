#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_TEXTURE_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_TEXTURE_HH_

#include <common/log/i_logger.hh>
#include <common/render/image.hh>
#include <common/render/texture_filter.hh>

#include "gl.hh"

class GlTexture final {
public:
	GlTexture(ILogger &logger, const Image &image, TextureFilter filter);
	~GlTexture();

	GlTexture(const GlTexture &) = delete;
	GlTexture &operator=(const GlTexture &) = delete;

private:
	ILogger &logger_;

public:
	const GLuint texture;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_TEXTURE_HH_
