#include <common/render/font.hh>

#include <cassert>

static const int CHARS_PER_COL  = 16;
static const int CHARS_PER_ROW = 16;

Font::Font(ILogger &logger, IAllocator &allocator, IFileSystem &file_system, ITextureFactory &texture_factory)
	: logger_(logger)
	, allocator_(allocator)
	, file_system_(file_system)
	, texture_(texture_factory)
	, char_width_(0)
	, char_height_(0)
	, char_uv_size_(0.0f, 0.0f)
{ }

bool Font::init(const char *const filename)
{
	assert(filename != nullptr);

	Texture &texture = texture_;

	{
		Image image(logger_, allocator_, file_system_);
		if (!image.init_from_tga(filename) || !texture.init(image, TextureFilter::NEAREST)) {
			return false;
		}
	}

	const std::uint16_t width = texture.get_width();
	const std::uint16_t height = texture.get_height();

	char_width_ = width / CHARS_PER_ROW;
	char_height_ = height / CHARS_PER_COL;
	char_uv_size_ = Vector2(static_cast<float>(char_width_) / width, static_cast<float>(char_height_) / height);
	return true;
}

const Texture &Font::get_texture() const
{
	return texture_;
}

std::uint16_t Font::get_char_width() const
{
	return char_width_;
}

std::uint16_t Font::get_char_height() const
{
	return char_height_;
}

Vector2 Font::get_char_uv_size() const
{
	return char_uv_size_;
}

Rect Font::get_char_uv(const char character) const
{
	const int row = character / CHARS_PER_ROW;
	const int col = character % CHARS_PER_ROW;
	return Rect(
		Vector2(
			static_cast<float>(col * char_width_) / texture_.get_width(),
			static_cast<float>((CHARS_PER_COL - row - 1) * char_height_) / texture_.get_height()
		),
		char_uv_size_
	);
}
