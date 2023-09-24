#include "gl_vertex_array.hh"

static GLuint create_vertex_array(ILogger &logger)
{
	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	logger.log(LogLevel::INFO, "Created OpenGL vertex array (%lu).", vertex_array);
	return vertex_array;
}

GlVertexArray::GlVertexArray(ILogger &logger)
	: logger_(logger)
	, vertex_array(create_vertex_array(logger))
{ }

GlVertexArray::~GlVertexArray()
{
	glDeleteBuffers(1, &vertex_array);
	logger_.log(LogLevel::INFO, "Destroyed OpenGL vertex array (%lu).", vertex_array);
}
