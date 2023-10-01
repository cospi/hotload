#ifndef HOTLOAD_COMMON_MATH_VECTOR3_HH_
#define HOTLOAD_COMMON_MATH_VECTOR3_HH_

#include <cmath>

struct Vector3 {
	Vector3() = default;

	Vector3(const float in_x, const float in_y, const float in_z)
		: x(in_x)
		, y(in_y)
		, z(in_z)
	{ }

	Vector3 &operator+=(const Vector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vector3 &operator-=(const Vector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vector3 &operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	float dot(const Vector3 &rhs) const
	{
		return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
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
		z /= length;
	}

	float x;
	float y;
	float z;
};

inline Vector3 operator+(Vector3 lhs, const Vector3 &rhs)
{
	return lhs += rhs;
}

inline Vector3 operator-(Vector3 lhs, const Vector3 &rhs)
{
	return lhs -= rhs;
}

inline Vector3 operator*(Vector3 lhs, const float rhs)
{
	return lhs *= rhs;
}

inline Vector3 operator*(const float lhs, const Vector3 rhs)
{
	return rhs * lhs;
}

#endif // HOTLOAD_COMMON_MATH_VECTOR3_HH_
