#ifndef HOTLOAD_COMMON_MATH_RECT_HH_
#define HOTLOAD_COMMON_MATH_RECT_HH_

#include "vector2.hh"

struct Rect {
	Rect() = default;

	Rect(const Vector2 &in_position, const Vector2 &in_size)
		: position(in_position)
		, size(in_size)
	{ }

	Vector2 get_min() const
	{
		return position;
	}

	Vector2 get_max() const
	{
		return position + size;
	}

	Vector2 position;
	Vector2 size;
};

#endif // HOTLOAD_COMMON_MATH_RECT_HH_
