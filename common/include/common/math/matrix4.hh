#ifndef HOTLOAD_COMMON_MATH_MATRIX4_HH_
#define HOTLOAD_COMMON_MATH_MATRIX4_HH_

#include <cstring>

struct Matrix4 {
	Matrix4() = default;

	static Matrix4 ortho(
		const float left,
		const float right,
		const float bottom,
		const float top,
		const float near,
		const float far
	)
	{
		Matrix4 matrix;
		std::memset(matrix.elements, 0, sizeof matrix.elements);
		float *const elements = matrix.elements;
		elements[0] = 2.0f / (right - left);
		elements[3] = (left + right) / (left - right);
		elements[5] = 2.0f / (top - bottom);
		elements[7] = (bottom + top) / (bottom - top);
		elements[10] = 2.0f / (near - far);
		elements[11] = (near + far) / (near - far);
		elements[15] = 1.0f;
		return matrix;
	}

	float elements[16];
};

#endif // HOTLOAD_COMMON_MATH_MATRIX4_HH_
