#include <common/render/image.hh>

#include <cassert>
#include <cstring>

#include <common/file_system/file.hh>
#include <common/file_system/path.hh>

static const unsigned char UNCOMPRESSED_TGA_HEADER[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static std::uint16_t parse_uint16_little_endian(const unsigned char *const data)
{
	assert(data != nullptr);

	return static_cast<std::uint16_t>(data[0] | (data[1] << 8));
}

static bool init_tga_path(Path &path, const char *const filename)
{
	assert(filename != nullptr);

	return path.init("res/textures/", filename, ".tga");
}

Image::Image(ILogger &logger, IAllocator &allocator, IFileSystem &file_system)
	: logger_(logger)
	, allocator_(allocator)
	, file_system_(file_system)
	, pixels_(nullptr)
	, width_(0)
	, height_(0)
{ }

Image::~Image()
{
	if (unsigned char *const pixels = pixels_; pixels != nullptr) {
		allocator_.free(pixels);
	}
}

bool Image::init_from_tga(const char *const filename)
{
	assert(pixels_ == nullptr);
	assert(filename != nullptr);

	Path path(allocator_);
	if (!init_tga_path(path, filename)) {
		return false;
	}

	File file(file_system_);
	if (!file.init(path.get_path(), "rb")) {
		return false;
	}

	std::size_t size;
	if (!file.try_get_size(&size)) {
		return false;
	}

	unsigned char header[18];
	if (!file.try_read(sizeof header, header)) {
		return false;
	}

	if (std::memcmp(header, UNCOMPRESSED_TGA_HEADER, sizeof UNCOMPRESSED_TGA_HEADER) != 0) {
		logger_.log(LogLevel::ERR, "Unsupported TGA header.");
		return false;
	}

	const unsigned char bytes_per_pixel = header[16] / 8;
	if ((bytes_per_pixel != 3 /* BGR */) && (bytes_per_pixel != 4 /* BGRA */)) {
		logger_.log(LogLevel::ERR, "Unsupported TGA bytes per pixel.");
		return false;
	}

	const std::uint16_t width = parse_uint16_little_endian(header + 12);
	const std::uint16_t height = parse_uint16_little_endian(header + 14);

	unsigned char *const pixels = static_cast<unsigned char *>(
		allocator_.allocate(static_cast<std::uint32_t>(width) * height * 4)
	);
	if (pixels == nullptr) {
		return false;
	}

	unsigned char *pixel = pixels;
	for (std::uint16_t y = 0; y < height; ++y) {
		for (std::uint16_t x = 0; x < width; ++x) {
			pixel[3] = 0xFF;
			if (!file.try_read(bytes_per_pixel, pixel)) {
				allocator_.free(pixels);
				return false;
			}
			// \note Convert BGRA to RGBA.
			const unsigned char tmp = pixel[0];
			pixel[0] = pixel[2];
			pixel[2] = tmp;
			pixel += 4;
		}
	}

	pixels_ = pixels;
	width_ = width;
	height_ = height;
	return true;
}

const unsigned char *Image::get_pixels() const
{
	assert(pixels_ != nullptr);

	return pixels_;
}

std::uint16_t Image::get_width() const
{
	assert(pixels_ != nullptr);

	return width_;
}

std::uint16_t Image::get_height() const
{
	assert(pixels_ != nullptr);

	return height_;
}
