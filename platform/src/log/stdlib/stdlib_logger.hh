#ifndef HOTLOAD_PLATFORM_STDLIB_STDLIB_LOGGER_HH_
#define HOTLOAD_PLATFORM_STDLIB_STDLIB_LOGGER_HH_

#include <cstdio>

#include <common/log/i_logger.hh>

class StdlibLogger final : public ILogger {
public:
	StdlibLogger(std::FILE *info_stream, std::FILE *warning_stream, std::FILE *error_stream);
	~StdlibLogger() override = default;

	StdlibLogger(const StdlibLogger &) = delete;
	StdlibLogger &operator=(const StdlibLogger &) = delete;

	void log(LogLevel level, const char *format, ...) override;

private:
	std::FILE *get_log_level_stream(LogLevel level) const;

	std::FILE *const info_stream_;
	std::FILE *const warning_stream_;
	std::FILE *const error_stream_;
};

#endif // HOTLOAD_PLATFORM_STDLIB_STDLIB_LOGGER_HH_
