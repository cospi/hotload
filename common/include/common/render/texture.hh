#ifndef HOTLOAD_COMMON_RENDER_TEXTURE_HH_
#define HOTLOAD_COMMON_RENDER_TEXTURE_HH_

#include "i_texture_factory.hh"

class Texture final {
public:
	explicit Texture(ITextureFactory &factory);
	~Texture();

	Texture(const Texture &) = delete;
	Texture &operator=(const Texture &) = delete;

	bool init(const Image &image);

	void *get_texture() const;

private:
	ITextureFactory &factory_;
	void *texture_;
};

#endif // HOTLOAD_COMMON_RENDER_TEXTURE_HH_
