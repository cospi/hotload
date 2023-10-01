#ifndef HOTLOAD_COMMON_RENDER_TEXT_HH_
#define HOTLOAD_COMMON_RENDER_TEXT_HH_

#include "font.hh"
#include "sprite_batch.hh"

void push_text(
	SpriteBatch &sprite_batch,
	const Font &font,
	const char *text,
	const Vector2 &position,
	float char_scale,
	float newline_offset
);

#endif // HOTLOAD_COMMON_RENDER_TEXT_HH_
