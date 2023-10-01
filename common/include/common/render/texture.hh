#ifndef HOTLOAD_COMMON_RENDER_TEXTURE_HH_
#define HOTLOAD_COMMON_RENDER_TEXTURE_HH_

#include "i_texture_factory.hh"

class Texture final {
public:
	explicit Texture(ITextureFactory &factory);
	~Texture();

	Texture(const Texture &) = delete;
	Texture &operator=(const Texture &) = delete;

	bool init(const Image &image, TextureFilter filter);

	void *get_texture() const;
	std::uint16_t get_width() const;
	std::uint16_t get_height() const;

private:
	ITextureFactory &factory_;
	void *texture_;
	std::uint16_t width_;
	std::uint16_t height_;
};

#endif // HOTLOAD_COMMON_RENDER_TEXTURE_HH_
