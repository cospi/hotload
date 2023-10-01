#include <common/render/text.hh>

#include <cassert>

void push_text(
	SpriteBatch &sprite_batch,
	const Font &font,
	const char *text,
	const Vector2 &position,
	const float char_scale,
	const float newline_offset
)
{
	assert(text != nullptr);

	const Vector2 size = Vector2(font.get_char_width() * char_scale, font.get_char_height() * char_scale);
	Sprite sprite(
		Rect(Vector2(position.x, position.y - size.y), size),
		Rect(Vector2(0.0f, 0.0f), font.get_char_uv_size())
	);

	char character;
	while ((character = *(text++)) != '\0') {
		if (character == '\n') {
			sprite.rect.position.x = position.x;
			sprite.rect.position.y -= size.y + newline_offset;
		} else {
			sprite.uv = font.get_char_uv(character);
			sprite_batch.push_sprite(sprite);
			sprite.rect.position.x += size.x;
		}
	}
}
