#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_VERTEX_ARRAY_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_VERTEX_ARRAY_HH_

#include <common/log/i_logger.hh>

#include "gl.hh"

class GlVertexArray final {
public:
	explicit GlVertexArray(ILogger &logger);
	~GlVertexArray();

	GlVertexArray(const GlVertexArray &) = delete;
	GlVertexArray &operator=(const GlVertexArray &) = delete;

private:
	ILogger &logger_;

public:
	const GLuint vertex_array;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_VERTEX_ARRAY_HH_
