#ifndef HOTLOAD_PLATFORM_FILE_SYSTEM_STDLIB_STDLIB_FILE_SYSTEM_HH_
#define HOTLOAD_PLATFORM_FILE_SYSTEM_STDLIB_STDLIB_FILE_SYSTEM_HH_

#include <common/file_system/i_file_system.hh>
#include <common/log/i_logger.hh>

class StdlibFileSystem final : public IFileSystem {
public:
	explicit StdlibFileSystem(ILogger &logger);
	~StdlibFileSystem() override = default;

	StdlibFileSystem(const StdlibFileSystem &) = delete;
	StdlibFileSystem &operator=(const StdlibFileSystem &) = delete;

	void *open_file(const char *path, const char *mode) override;
	void close_file(void *file) override;
	bool try_get_file_size(void *file, std::size_t *out_size) override;

private:
	ILogger &logger_;
};

#endif // HOTLOAD_PLATFORM_FILE_SYSTEM_STDLIB_STDLIB_FILE_SYSTEM_HH_
