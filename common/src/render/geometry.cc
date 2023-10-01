#include <common/render/geometry.hh>

#include <cassert>

void geometry_init_cube(Vertex *const vertices, std::uint16_t *const indices, float extents)
{
	assert(vertices != nullptr);
	assert(indices != nullptr);

	// Front
	vertices[0] = Vertex(Vector3(-extents, -extents, extents), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	vertices[1] = Vertex(Vector3(extents, -extents, extents), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	vertices[2] = Vertex(Vector3(extents, extents, extents), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	vertices[3] = Vertex(Vector3(-extents, extents, extents), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));
	// Back
	vertices[4] = Vertex(Vector3(extents, -extents, -extents), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f));
	vertices[5] = Vertex(Vector3(-extents, -extents, -extents), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f));
	vertices[6] = Vertex(Vector3(-extents, extents, -extents), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f));
	vertices[7] = Vertex(Vector3(extents, extents, -extents), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f));
	// Top
	vertices[8] = Vertex(Vector3(-extents, extents, extents), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f));
	vertices[9] = Vertex(Vector3(extents, extents, extents), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f));
	vertices[10] = Vertex(Vector3(extents, extents, -extents), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f));
	vertices[11] = Vertex(Vector3(-extents, extents, -extents), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f));
	// Bottom
	vertices[12] = Vertex(Vector3(extents, -extents, -extents), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f));
	vertices[13] = Vertex(Vector3(-extents, -extents, -extents), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f));
	vertices[14] = Vertex(Vector3(-extents, -extents, extents), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f));
	vertices[15] = Vertex(Vector3(extents, -extents, extents), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f));
	// Left
	vertices[16] = Vertex(Vector3(-extents, -extents, -extents), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f));
	vertices[17] = Vertex(Vector3(-extents, -extents, extents), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f));
	vertices[18] = Vertex(Vector3(-extents, extents, extents), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f));
	vertices[19] = Vertex(Vector3(-extents, extents, -extents), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f));
	// Right
	vertices[20] = Vertex(Vector3(extents, -extents, extents), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f));
	vertices[21] = Vertex(Vector3(extents, -extents, -extents), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f));
	vertices[22] = Vertex(Vector3(extents, extents, -extents), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f));
	vertices[23] = Vertex(Vector3(extents, extents, extents), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f));

	for (std::size_t i = 0; i < 6; ++i) {
		const uint16_t vertex_offset = static_cast<std::uint16_t>(i * 4);
		const std::size_t indices_offset = i * 6;
		indices[indices_offset] = vertex_offset;
		indices[indices_offset + 1] = static_cast<std::uint16_t>(vertex_offset + 1);
		indices[indices_offset + 2] = static_cast<std::uint16_t>(vertex_offset + 2);
		indices[indices_offset + 3] = vertex_offset;
		indices[indices_offset + 4] = static_cast<std::uint16_t>(vertex_offset + 2);
		indices[indices_offset + 5] = static_cast<std::uint16_t>(vertex_offset + 3);
	}
}
