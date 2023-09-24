#include "gl_buffer.hh"

static GLuint create_buffer(ILogger &logger)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	logger.log(LogLevel::INFO, "Created OpenGL buffer (%lu).", buffer);
	return buffer;
}

GlBuffer::GlBuffer(ILogger &logger)
	: logger_(logger)
	, buffer(create_buffer(logger))
{ }

GlBuffer::~GlBuffer()
{
	glDeleteBuffers(1, &buffer);
	logger_.log(LogLevel::INFO, "Destroyed OpenGL buffer (%lu).", buffer);
}
