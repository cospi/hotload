#ifndef HOTLOAD_COMMON_RENDER_IMAGE_HH_
#define HOTLOAD_COMMON_RENDER_IMAGE_HH_

#include <cstdint>

#include "../file_system/i_file_system.hh"
#include "../log/i_logger.hh"
#include "../memory/i_allocator.hh"

class Image final {
public:
	Image(ILogger &logger, IAllocator &allocator, IFileSystem &file_system);
	~Image();

	Image(const Image &) = delete;
	Image &operator=(const Image &) = delete;

	bool init_from_tga(const char *filename);

	const unsigned char *get_pixels() const;
	std::uint16_t get_width() const;
	std::uint16_t get_height() const;

private:
	ILogger &logger_;
	IAllocator &allocator_;
	IFileSystem &file_system_;
	unsigned char *pixels_;
	std::uint16_t width_;
	std::uint16_t height_;
};

#endif // HOTLOAD_COMMON_RENDER_IMAGE_HH_
