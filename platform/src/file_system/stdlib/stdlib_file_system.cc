#include "stdlib_file_system.hh"

#include <cassert>
#include <cinttypes>
#include <cstdio>

StdlibFileSystem::StdlibFileSystem(ILogger &logger)
	: logger_(logger)
{ }

void *StdlibFileSystem::open_file(const char *const path, const char *const mode)
{
	assert(path != nullptr);
	assert(mode != nullptr);

	std::FILE *const stdlib_file = std::fopen(path, mode);
	if (stdlib_file == nullptr) {
		logger_.log(LogLevel::ERROR, "Opening file \"%s\" failed.", path);
		return nullptr;
	}

	void *const file = static_cast<void *>(stdlib_file);
	logger_.log(
		LogLevel::INFO,
		"Opened file \"%s\" (%" PRIxPTR ").",
		path,
		reinterpret_cast<std::uintptr_t>(file)
	);
	return file;
}

void StdlibFileSystem::close_file(void *const file)
{
	assert(file != nullptr);

	std::fclose(static_cast<std::FILE *>(file));
	logger_.log(LogLevel::INFO, "Closed file (%" PRIxPTR ").", reinterpret_cast<std::uintptr_t>(file));
}

bool StdlibFileSystem::try_get_file_size(void *const file, std::size_t *const out_size)
{
	assert(file != nullptr);
	assert(out_size != nullptr);

	std::FILE *const stdlib_file = static_cast<std::FILE *>(file);

	const long original_position = std::ftell(stdlib_file);
	if (original_position == -1L) {
		logger_.log(LogLevel::ERROR, "Getting file position failed.");
		return false;
	}

	if (std::fseek(stdlib_file, 0L, SEEK_END) != 0) {
		logger_.log(LogLevel::ERROR, "Navigating to file end failed.");
		return false;
	}

	const long size = std::ftell(stdlib_file);
	std::fseek(stdlib_file, original_position, SEEK_SET);
	if (size == -1L) {
		logger_.log(LogLevel::ERROR, "Getting file size failed.");
		return false;
	}

	*out_size = static_cast<std::size_t>(size);
	return true;
}
