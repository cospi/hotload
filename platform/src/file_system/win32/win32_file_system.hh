#ifndef HOTLOAD_PLATFORM_FILE_SYSTEM_WIN32_WIN32_FILE_SYSTEM_HH_
#define HOTLOAD_PLATFORM_FILE_SYSTEM_WIN32_WIN32_FILE_SYSTEM_HH_

#include <common/file_system/i_file_system.hh>
#include <common/log/i_logger.hh>

class Win32FileSystem final : public IFileSystem {
public:
	explicit Win32FileSystem(ILogger &logger);
	~Win32FileSystem() override = default;

	Win32FileSystem(const Win32FileSystem &) = delete;
	Win32FileSystem &operator=(const Win32FileSystem &) = delete;

	void *open_file(const char *path, const char *mode) override;
	void close_file(void *file) override;
	bool try_get_file_size(void *file, std::size_t *out_size) override;
	bool try_read_file(void *file, std::size_t size, unsigned char *buffer) override;
	bool try_get_file_last_modification_time(const char *path, std::time_t *out_last_modification_time) override;

private:
	ILogger &logger_;
};

#endif // HOTLOAD_PLATFORM_FILE_SYSTEM_WIN32_WIN32_FILE_SYSTEM_HH_
