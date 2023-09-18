#ifndef HOTLOAD_COMMON_FILE_SYSTEM_I_FILE_SYSTEM_HH_
#define HOTLOAD_COMMON_FILE_SYSTEM_I_FILE_SYSTEM_HH_

#include <cstddef>

class IFileSystem {
public:
	virtual ~IFileSystem() = default;

	virtual void *open_file(const char *path, const char *mode) = 0;
	virtual void close_file(void *file) = 0;
	virtual bool try_get_file_size(void *file, std::size_t *out_size) = 0;
};

#endif // HOTLOAD_COMMON_FILE_SYSTEM_I_FILE_SYSTEM_HH_
