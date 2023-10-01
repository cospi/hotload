#include <common/render/texture.hh>

#include <cassert>

Texture::Texture(ITextureFactory &factory)
	: factory_(factory)
	, texture_(nullptr)
	, width_(0)
	, height_(0)
{ }

Texture::~Texture()
{
	if (void *const texture = texture_; texture != nullptr) {
		factory_.destroy_texture(texture);
	}
}

bool Texture::init(const Image &image, const TextureFilter filter)
{
	assert(texture_ == nullptr);

	void *const texture = factory_.create_texture(image, filter);
	if (texture == nullptr) {
		return false;
	}

	texture_ = texture;
	width_ = image.get_width();
	height_ = image.get_height();
	return true;
}

void *Texture::get_texture() const
{
	assert(texture_ != nullptr);

	return texture_;
}

std::uint16_t Texture::get_width() const
{
	assert(texture_ != nullptr);

	return width_;
}

std::uint16_t Texture::get_height() const
{
	assert(texture_ != nullptr);

	return height_;
}
