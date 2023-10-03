#ifndef HOTLOAD_COMMON_FILE_SYSTEM_I_FILE_SYSTEM_HH_
#define HOTLOAD_COMMON_FILE_SYSTEM_I_FILE_SYSTEM_HH_

#include <cstddef>
#include <ctime>

class IFileSystem {
public:
	virtual ~IFileSystem() = default;

	virtual void *open_file(const char *path, const char *mode) = 0;
	virtual void close_file(void *file) = 0;
	virtual bool try_get_file_size(void *file, std::size_t *out_size) = 0;
	virtual bool try_read_file(void *file, std::size_t size, unsigned char *buffer) = 0;
	virtual bool try_get_file_last_modification_time(
		const char *path,
		std::time_t *out_last_modification_time
	) = 0;
};

#endif // HOTLOAD_COMMON_FILE_SYSTEM_I_FILE_SYSTEM_HH_
