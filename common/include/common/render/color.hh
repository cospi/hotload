#ifndef HOTLOAD_COMMON_RENDER_COLOR_HH_
#define HOTLOAD_COMMON_RENDER_COLOR_HH_

struct Color {
	Color() = default;

	Color(const float in_r, const float in_g, const float in_b, const float in_a)
		: r(in_r)
		, g(in_g)
		, b(in_b)
		, a(in_a)
	{ }

	float r;
	float g;
	float b;
	float a;
};

#endif // HOTLOAD_COMMON_RENDER_COLOR_HH_
