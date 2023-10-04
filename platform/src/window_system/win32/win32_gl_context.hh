#ifndef HOTLOAD_PLATFORM_WINDOW_SYSTEM_WIN32_WIN32_GL_CONTEXT_HH_
#define HOTLOAD_PLATFORM_WINDOW_SYSTEM_WIN32_WIN32_GL_CONTEXT_HH_

#include <Windows.h>

#include <common/log/i_logger.hh>

class Win32GlContext final {
public:
	explicit Win32GlContext(ILogger &logger);
	~Win32GlContext();

	Win32GlContext(const Win32GlContext &) = delete;
	Win32GlContext &operator=(const Win32GlContext &) = delete;

	bool init(HDC device_context);

	HGLRC get_context() const;

private:
	ILogger &logger_;
	HGLRC context_;
};

#endif // HOTLOAD_PLATFORM_WINDOW_SYSTEM_WIN32_WIN32_GL_CONTEXT_HH_
