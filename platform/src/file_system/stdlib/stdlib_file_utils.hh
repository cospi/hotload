#ifndef HOTLOAD_PLATFORM_FILE_SYSTEM_STDLIB_STDLIB_FILE_UTILS_HH_
#define HOTLOAD_PLATFORM_FILE_SYSTEM_STDLIB_STDLIB_FILE_UTILS_HH_

#include <cstddef>

#include <common/log/i_logger.hh>

void *stdlib_file_open(ILogger &logger, const char *path, const char *mode);
void stdlib_file_close(ILogger &logger, void *file);
bool stdlib_file_try_get_size(ILogger &logger, void *file, std::size_t *out_size);
bool stdlib_file_try_read(ILogger &logger, void *file, std::size_t size, unsigned char *buffer);

#endif // HOTLOAD_PLATFORM_FILE_SYSTEM_STDLIB_STDLIB_FILE_UTILS_HH_
