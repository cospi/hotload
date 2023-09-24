#ifndef HOTLOAD_COMMON_FILE_SYSTEM_PATH_HH_
#define HOTLOAD_COMMON_FILE_SYSTEM_PATH_HH_

#include <common/memory/i_allocator.hh>

class Path final {
public:
	explicit Path(IAllocator &allocator);
	~Path();

	Path(const Path &) = delete;
	Path &operator=(const Path &) = delete;

	bool init(const char *directory, const char *filename, const char *extension);

	const char *get_path() const;

private:
	IAllocator &allocator_;
	char *path_;
};

#endif // HOTLOAD_COMMON_FILE_SYSTEM_PATH_HH_
