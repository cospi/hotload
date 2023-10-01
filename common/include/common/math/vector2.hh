#ifndef HOTLOAD_COMMON_MATH_VECTOR2_HH_
#define HOTLOAD_COMMON_MATH_VECTOR2_HH_

#include <cmath>

struct Vector2 {
	Vector2() = default;

	Vector2(const float in_x, const float in_y)
		: x(in_x)
		, y(in_y)
	{ }

	Vector2 &operator+=(const Vector2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2 &operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	float dot(const Vector2 &rhs) const
	{
		return (x * rhs.x) + (y * rhs.y);
	}

	float get_length() const
	{
		return std::sqrt(dot(*this));
	}

	void normalize()
	{
		const float length = get_length();
		x /= length;
		y /= length;
	}

	float x;
	float y;
};

inline Vector2 operator+(Vector2 lhs, const Vector2 &rhs)
{
	return lhs += rhs;
}

#endif // HOTLOAD_COMMON_MATH_VECTOR2_HH_
