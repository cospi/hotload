#ifndef HOTLOAD_COMMON_MATH_VECTOR3_HH_
#define HOTLOAD_COMMON_MATH_VECTOR3_HH_

struct Vector3 {
	Vector3() = default;

	Vector3(const float in_x, const float in_y, const float in_z)
		: x(in_x)
		, y(in_y)
		, z(in_z)
	{ }

	float x;
	float y;
	float z;
};

#endif // HOTLOAD_COMMON_MATH_VECTOR3_HH_
