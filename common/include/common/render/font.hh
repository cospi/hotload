#ifndef HOTLOAD_COMMON_RENDER_FONT_HH_
#define HOTLOAD_COMMON_RENDER_FONT_HH_

#include "texture.hh"
#include "../math/rect.hh"

class Font final {
public:
	Font(ILogger &logger, IAllocator &allocator, IFileSystem &file_system, ITextureFactory &texture_factory);
	~Font() = default;

	Font(const Font &) = delete;
	Font &operator=(const Font &) = delete;

	bool init(const char *filename);

	const Texture &get_texture() const;
	std::uint16_t get_char_width() const;
	std::uint16_t get_char_height() const;
	Vector2 get_char_uv_size() const;
	Rect get_char_uv(char character) const;

private:
	ILogger &logger_;
	IAllocator &allocator_;
	IFileSystem &file_system_;
	Texture texture_;
	std::uint16_t char_width_;
	std::uint16_t char_height_;
	Vector2 char_uv_size_;
};

#endif // HOTLOAD_COMMON_RENDER_FONT_HH_
