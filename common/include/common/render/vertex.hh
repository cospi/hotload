#ifndef HOTLOAD_COMMON_RENDER_VERTEX_HH_
#define HOTLOAD_COMMON_RENDER_VERTEX_HH_

#include "../math/vector2.hh"
#include "../math/vector3.hh"

struct Vertex {
	Vertex() = default;

	Vertex(const Vector3 &in_position, const Vector2 &in_uv)
		: position(in_position)
		, uv(in_uv)
	{ }

	Vector3 position;
	Vector2 uv;
};

#endif // HOTLOAD_COMMON_RENDER_VERTEX_HH_
