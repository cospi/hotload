#ifndef HOTLOAD_COMMON_RENDER_SPRITE_HH_
#define HOTLOAD_COMMON_RENDER_SPRITE_HH_

#include "../math/rect.hh"

struct Sprite {
	Sprite() = default;

	Sprite(const Rect &in_rect, const Rect &in_uv)
		: rect(in_rect)
		, uv(in_uv)
	{ }

	Rect rect;
	Rect uv;
};

#endif // HOTLOAD_COMMON_RENDER_SPRITE_HH_
