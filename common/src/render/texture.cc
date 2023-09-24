#include <common/render/texture.hh>

#include <cassert>

Texture::Texture(ITextureFactory &factory)
	: factory_(factory)
	, texture_(nullptr)
{ }

Texture::~Texture()
{
	if (void *const texture = texture_; texture != nullptr) {
		factory_.destroy_texture(texture);
	}
}

bool Texture::init(const Image &image)
{
	assert(texture_ == nullptr);

	void *const texture = factory_.create_texture(image);
	if (texture == nullptr) {
		return false;
	}

	texture_ = texture;
	return true;
}

void *Texture::get_texture() const
{
	assert(texture_ != nullptr);

	return texture_;
}
