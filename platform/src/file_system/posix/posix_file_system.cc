#include "posix_file_system.hh"

#include <sys/stat.h>

#include <cassert>
#include <cinttypes>
#include <cstdio>

#include "../stdlib/stdlib_file_utils.hh"

PosixFileSystem::PosixFileSystem(ILogger &logger)
	: logger_(logger)
{ }

void *PosixFileSystem::open_file(const char *const path, const char *const mode)
{
	assert(path != nullptr);
	assert(mode != nullptr);

	return stdlib_file_open(logger_, path, mode);
}

void PosixFileSystem::close_file(void *const file)
{
	assert(file != nullptr);

	stdlib_file_close(logger_, file);
}

bool PosixFileSystem::try_get_file_size(void *const file, std::size_t *const out_size)
{
	assert(file != nullptr);
	assert(out_size != nullptr);

	return stdlib_file_try_get_size(logger_, file, out_size);
}

bool PosixFileSystem::try_get_file_last_modification_time(
	void *const file,
	std::time_t *const out_last_modification_time
)
{
	assert(file != nullptr);
	assert(out_last_modification_time != nullptr);

	const int fd = fileno(static_cast<std::FILE *>(file));
	if (fd == -1) {
		logger_.log(LogLevel::ERROR, "Getting file descriptor failed.");
		return false;
	}

	struct stat status;
	if (fstat(fd, &status) == -1) {
		logger_.log(LogLevel::ERROR, "Getting file status failed.");
		return false;
	}

	*out_last_modification_time = status.st_mtime;
	return true;
}

bool PosixFileSystem::try_read_file(void *const file, const std::size_t size, unsigned char *const buffer)
{
	assert(file != nullptr);
	assert(size > 0);
	assert(buffer != nullptr);

	return stdlib_file_try_read(logger_, file, size, buffer);
}
