#include "gl_texture_factory.hh"

#include <cassert>
#include <cinttypes>
#include <new>

#include "gl_texture.hh"

GlTextureFactory::GlTextureFactory(ILogger &logger, IAllocator &allocator)
	: logger_(logger)
	, allocator_(allocator)
{ }

void *GlTextureFactory::create_texture(const Image &image, const TextureFilter filter)
{
	void *const texture_memory = allocator_.allocate(sizeof(GlTexture));
	if (texture_memory == nullptr) {
		return nullptr;
	}

	new(texture_memory) GlTexture(logger_, image, filter);
	return texture_memory;
}

void GlTextureFactory::destroy_texture(void *const texture)
{
	assert(texture != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(texture) % alignof(GlTexture)) == 0);

	static_cast<GlTexture *>(texture)->~GlTexture();
	allocator_.free(texture);
}
