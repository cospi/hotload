#ifndef HOTLOAD_COMMON_RENDER_VERTEX_HH_
#define HOTLOAD_COMMON_RENDER_VERTEX_HH_

#include "../math/vector2.hh"
#include "../math/vector3.hh"

struct Vertex {
	Vertex() = default;

	Vertex(const Vector3 &in_position, const Vector3 &in_normal, const Vector2 &in_uv)
		: position(in_position)
		, normal(in_normal)
		, uv(in_uv)
	{ }

	Vector3 position;
	Vector3 normal;
	Vector2 uv;
};

#endif // HOTLOAD_COMMON_RENDER_VERTEX_HH_
