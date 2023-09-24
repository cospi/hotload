#include "stdlib_file_utils.hh"

#include <cassert>
#include <cinttypes>
#include <cstdio>

void *stdlib_file_open(ILogger &logger, const char *const path, const char *const mode)
{
	assert(path != nullptr);
	assert(mode != nullptr);

	std::FILE *const stdlib_file = std::fopen(path, mode);
	if (stdlib_file == nullptr) {
		logger.log(LogLevel::ERROR, "Opening file \"%s\" failed.", path);
		return nullptr;
	}

	void *const file = static_cast<void *>(stdlib_file);
	logger.log(
		LogLevel::INFO,
		"Opened file \"%s\" (%" PRIxPTR ").",
		path,
		reinterpret_cast<std::uintptr_t>(file)
	);
	return file;
}

void stdlib_file_close(ILogger &logger, void *const file)
{
	assert(file != nullptr);

	std::fclose(static_cast<std::FILE *>(file));
	logger.log(LogLevel::INFO, "Closed file (%" PRIxPTR ").", reinterpret_cast<std::uintptr_t>(file));
}

bool stdlib_file_try_get_size(ILogger &logger, void *const file, std::size_t *const out_size)
{
	assert(file != nullptr);
	assert(out_size != nullptr);

	std::FILE *const stdlib_file = static_cast<std::FILE *>(file);

	const long original_position = std::ftell(stdlib_file);
	if (original_position == -1L) {
		logger.log(LogLevel::ERROR, "Getting file position failed.");
		return false;
	}

	if (std::fseek(stdlib_file, 0L, SEEK_END) != 0) {
		logger.log(LogLevel::ERROR, "Navigating to file end failed.");
		return false;
	}

	const long size = std::ftell(stdlib_file);
	std::fseek(stdlib_file, original_position, SEEK_SET);
	if (size == -1L) {
		logger.log(LogLevel::ERROR, "Getting file size failed.");
		return false;
	}

	*out_size = static_cast<std::size_t>(size);
	return true;
}

bool stdlib_file_try_read(ILogger &logger, void *const file, const std::size_t size, unsigned char *const buffer)
{
	assert(file != nullptr);
	assert(size > 0);
	assert(buffer != nullptr);

	std::FILE *const stdlib_file = static_cast<std::FILE *>(file);
	if (std::fread(buffer, size, 1, stdlib_file) != 1) {
		logger.log(LogLevel::ERROR, "Reading %zu bytes from file failed.", size);
		return false;
	}
	return true;
}
