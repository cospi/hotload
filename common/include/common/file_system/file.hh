#ifndef HOTLOAD_COMMON_FILE_SYSTEM_FILE_HH_
#define HOTLOAD_COMMON_FILE_SYSTEM_FILE_HH_

#include "i_file_system.hh"

class File final {
public:
	explicit File(IFileSystem &file_system);
	~File();

	File(const File &) = delete;
	File &operator=(const File &) = delete;

	bool init(const char *path, const char *mode);

	bool try_get_size(std::size_t *out_size) const;
	bool try_read(std::size_t size, unsigned char *buffer) const;

private:
	IFileSystem &file_system_;
	void *file_;
};

#endif // HOTLOAD_COMMON_FILE_SYSTEM_FILE_HH_
