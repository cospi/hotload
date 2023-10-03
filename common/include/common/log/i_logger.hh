#ifndef HOTLOAD_COMMON_LOG_I_LOGGER_HH_
#define HOTLOAD_COMMON_LOG_I_LOGGER_HH_

enum class LogLevel {
	INFO,
	WARN,
	ERR
};

class ILogger {
public:
	virtual ~ILogger() = default;

	virtual void log(LogLevel level, const char *format, ...) = 0;
};

#endif // HOTLOAD_COMMON_LOG_I_LOGGER_HH_
