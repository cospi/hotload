#ifndef HOTLOAD_COMMON_MATH_MATRIX4_HH_
#define HOTLOAD_COMMON_MATH_MATRIX4_HH_

#include <cmath>
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

	static Matrix4 perspective(const float fov, const float aspect_ratio, const float near, const float far)
	{
		const float tan_fov_half_inv = 1.0f / std::tan(fov * 0.5f);
		const float near_minus_far_inv = 1.0f / (near - far);

		Matrix4 matrix;
		std::memset(matrix.elements, 0, sizeof matrix.elements);
		float *const elements = matrix.elements;
		elements[0] = tan_fov_half_inv / aspect_ratio;
		elements[5] = tan_fov_half_inv;
		elements[10] = (near + far) * near_minus_far_inv;
		elements[11] = near * far * 2.0f * near_minus_far_inv;
		elements[14] = -1.0f;
		elements[15] = 1.0f;
		return matrix;
	}

	static Matrix4 translate(const Vector3 &translate)
	{
		Matrix4 matrix;
		std::memset(matrix.elements, 0, sizeof matrix.elements);
		float *const elements = matrix.elements;
		elements[0] = 1.0f;
		elements[3] = translate.x;
		elements[5] = 1.0f;
		elements[7] = translate.y;
		elements[10] = 1.0f;
		elements[11] = translate.z;
		elements[15] = 1.0f;
		return matrix;
	}

	float elements[16];
};

#endif // HOTLOAD_COMMON_MATH_MATRIX4_HH_
