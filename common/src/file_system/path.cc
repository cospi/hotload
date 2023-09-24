#include <common/file_system/path.hh>

#include <cassert>
#include <cstdio>
#include <cstring>

static char *create_path(
	const char *const directory,
	const char *const filename,
	const char *const extension,
	IAllocator &allocator
)
{
	assert(directory != nullptr);
	assert(filename != nullptr);
	assert(extension != nullptr);

	const std::size_t filename_start_index = std::strlen(directory);
	const std::size_t extension_start_index = filename_start_index + std::strlen(filename);
	const std::size_t length = extension_start_index + std::strlen(extension);

	void *const path_memory = allocator.allocate(length + 1);
	if (path_memory == nullptr) {
		return nullptr;
	}

	char *const path = static_cast<char *>(path_memory);
	std::sprintf(path, "%s", directory);
	std::sprintf(path + filename_start_index, "%s", filename);
	std::sprintf(path + extension_start_index, "%s", extension);
	path[length] = '\0';
	return path;
}

Path::Path(IAllocator &allocator)
	: allocator_(allocator)
	, path_(nullptr)
{ }

Path::~Path()
{
	if (char *const path = path_; path != nullptr) {
		allocator_.free(path);
	}
}

bool Path::init(const char *const directory, const char *const filename, const char *const extension)
{
	assert(path_ == nullptr);
	assert(directory != nullptr);
	assert(filename != nullptr);
	assert(extension != nullptr);

	char *const path = create_path(directory, filename, extension, allocator_);
	if (path == nullptr) {
		return false;
	}

	path_ = path;
	return true;
}

const char *Path::get_path() const
{
	assert(path_ != nullptr);

	return path_;
}
