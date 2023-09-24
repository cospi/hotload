#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_BUFFER_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_BUFFER_HH_

#include <common/log/i_logger.hh>

#include "gl.hh"

class GlBuffer final {
public:
	explicit GlBuffer(ILogger &logger);
	~GlBuffer();

	GlBuffer(const GlBuffer &) = delete;
	GlBuffer &operator=(const GlBuffer &) = delete;

private:
	ILogger &logger_;

public:
	const GLuint buffer;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_BUFFER_HH_
