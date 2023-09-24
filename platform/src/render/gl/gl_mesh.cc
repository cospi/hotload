#include "gl_mesh.hh"

#include <cassert>
#include <cstddef>

GlMesh::GlMesh(
	ILogger &logger,
	const Vertex *const vertices,
	const std::size_t vertex_count,
	const std::uint16_t *const indices,
	const std::size_t in_index_count
)
	: vertex_array_(logger)
	, vertex_buffer_(logger)
	, index_buffer_(logger)
	, index_count(in_index_count)
{
	assert(vertices != nullptr);
	assert(vertex_count > 0);
	assert(indices != nullptr);
	assert(in_index_count > 0);

	const GLuint vertex_array = vertex_array_.vertex_array;
	glBindVertexArray(vertex_array);

	const GLuint vertex_buffer = vertex_buffer_.buffer;
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(vertex_count * sizeof(Vertex)),
		vertices,
		GL_STATIC_DRAW
	);

	const GLuint index_buffer = index_buffer_.buffer;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(in_index_count * sizeof(std::uint16_t)),
		indices,
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		static_cast<GLvoid *>(offsetof(Vertex, position))
	);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<GLvoid *>(offsetof(Vertex, uv))
	);
}

const GlVertexArray &GlMesh::get_vertex_array() const
{
	return vertex_array_;
}
