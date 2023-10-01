#include "gl_texture.hh"

#include <cassert>

static GLint convert_filter(const TextureFilter filter)
{
	switch (filter) {
	case TextureFilter::LINEAR:
		return GL_LINEAR;
	case TextureFilter::NEAREST:
		return GL_NEAREST;
	default:
		assert(false);
		return 0;
	}
}

static GLuint create_texture(ILogger &logger, const Image &image, const TextureFilter filter)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		static_cast<GLsizei>(image.get_width()),
		static_cast<GLsizei>(image.get_height()),
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image.get_pixels()
	);
	const GLint filter_param = convert_filter(filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	logger.log(LogLevel::INFO, "Created OpenGL texture (%lu).", texture);
	return texture;
}

GlTexture::GlTexture(ILogger &logger, const Image &image, const TextureFilter filter)
	: logger_(logger)
	, texture(create_texture(logger, image, filter))
{ }

GlTexture::~GlTexture()
{
	glDeleteTextures(1, &texture);
	logger_.log(LogLevel::INFO, "Destroyed OpenGL texture (%lu).", texture);
}
