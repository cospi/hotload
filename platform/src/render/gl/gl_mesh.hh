#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_MESH_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_MESH_HH_

#include <cstdint>

#include <common/render/vertex.hh>

#include "gl_buffer.hh"
#include "gl_vertex_array.hh"

class GlMesh final {
public:
	GlMesh(
		ILogger &logger,
		const Vertex *vertices,
		std::size_t vertex_count,
		const std::uint16_t *indices,
		std::size_t in_index_count
	);
	~GlMesh() = default;

	GlMesh(const GlMesh &) = delete;
	GlMesh &operator=(const GlMesh &) = delete;

	const GlVertexArray &get_vertex_array() const;

private:
	GlVertexArray vertex_array_;
	GlBuffer vertex_buffer_;
	GlBuffer index_buffer_;

public:
	const std::size_t index_count;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_MESH_HH_
