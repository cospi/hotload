#ifndef HOTLOAD_COMMON_MATH_VECTOR2_HH_
#define HOTLOAD_COMMON_MATH_VECTOR2_HH_

struct Vector2 {
	Vector2() = default;

	Vector2(const float in_x, const float in_y)
		: x(in_x)
		, y(in_y)
	{ }

	float x;
	float y;
};

#endif // HOTLOAD_COMMON_MATH_VECTOR2_HH_
