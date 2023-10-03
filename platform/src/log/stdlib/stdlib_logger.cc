#include "stdlib_logger.hh"

#include <cassert>
#include <cstdarg>

static const char *get_log_level_name(const LogLevel level)
{
	switch (level) {
	case LogLevel::INFO:
		return "Info";
	case LogLevel::WARN:
		return "Warning";
	case LogLevel::ERR:
		return "Error";
	default:
		assert(false);
		return "Unknown";
	}
}

StdlibLogger::StdlibLogger(
	std::FILE *const info_stream,
	std::FILE *const warning_stream,
	std::FILE *const error_stream
)
	: info_stream_(info_stream)
	, warning_stream_(warning_stream)
	, error_stream_(error_stream)
{ }

void StdlibLogger::log(const LogLevel level, const char *const format, ...)
{
	assert(format != nullptr);

	std::FILE *const stream = get_log_level_stream(level);
	if (stream == nullptr) {
		return;
	}

	std::fputc('[', stream);
	std::fputs(get_log_level_name(level), stream);
	std::fputs("] ", stream);

	std::va_list args;
	va_start(args, format);
	std::vfprintf(stream, format, args);
	va_end(args);

	std::fputc('\n', stream);
	std::fflush(stream);
}

std::FILE *StdlibLogger::get_log_level_stream(const LogLevel level) const
{
	switch (level) {
	case LogLevel::INFO:
		return info_stream_;
	case LogLevel::WARN:
		return warning_stream_;
	case LogLevel::ERR:
		return error_stream_;
	default:
		assert(false);
		return nullptr;
	}
}
