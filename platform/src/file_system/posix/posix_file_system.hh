#ifndef HOTLOAD_PLATFORM_FILE_SYSTEM_POSIX_POSIX_FILE_SYSTEM_HH_
#define HOTLOAD_PLATFORM_FILE_SYSTEM_POSIX_POSIX_FILE_SYSTEM_HH_

#include <common/file_system/i_file_system.hh>
#include <common/log/i_logger.hh>

class PosixFileSystem final : public IFileSystem {
public:
	explicit PosixFileSystem(ILogger &logger);
	~PosixFileSystem() override = default;

	PosixFileSystem(const PosixFileSystem &) = delete;
	PosixFileSystem &operator=(const PosixFileSystem &) = delete;

	void *open_file(const char *path, const char *mode) override;
	void close_file(void *file) override;
	bool try_get_file_size(void *file, std::size_t *out_size) override;
	bool try_get_file_last_modification_time(void *file, std::time_t *out_last_modification_time) override;

private:
	ILogger &logger_;
};

#endif // HOTLOAD_PLATFORM_FILE_SYSTEM_POSIX_POSIX_FILE_SYSTEM_HH_
