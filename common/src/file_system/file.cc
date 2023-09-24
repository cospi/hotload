#include <common/file_system/file.hh>

#include <cassert>

File::File(IFileSystem &file_system)
	: file_system_(file_system)
	, file_(nullptr)
{ }

File::~File()
{
	if (void *const file = file_; file != nullptr) {
		file_system_.close_file(file);
	}
}

bool File::init(const char *const path, const char *const mode)
{
	assert(file_ == nullptr);
	assert(path != nullptr);
	assert(mode != nullptr);

	void *const file = file_system_.open_file(path, mode);
	if (file == nullptr) {
		return false;
	}

	file_ = file;
	return true;
}

bool File::try_get_size(std::size_t *const out_size) const
{
	assert(file_ != nullptr);
	assert(out_size != nullptr);

	return file_system_.try_get_file_size(file_, out_size);
}

bool File::try_read(std::size_t size, unsigned char *const buffer) const
{
	assert(file_ != nullptr);
	assert(size > 0);
	assert(buffer != nullptr);

	return file_system_.try_read_file(file_, size, buffer);
}
