#ifndef HOTLOAD_PLATFORM_WINDOW_SYSTEM_WIN32_WIN32_GL_WINDOW_HH_
#define HOTLOAD_PLATFORM_WINDOW_SYSTEM_WIN32_WIN32_GL_WINDOW_HH_

#include <Windows.h>

#include <common/log/i_logger.hh>

class Win32GlWindow final {
public:
	explicit Win32GlWindow(ILogger &logger);
	~Win32GlWindow();

	Win32GlWindow(const Win32GlWindow &) = delete;
	Win32GlWindow &operator=(const Win32GlWindow &) = delete;

	bool init(
		unsigned int width,
		unsigned int height,
		const wchar_t *title,
		UINT style,
		WNDPROC proc,
		HINSTANCE instance,
		HCURSOR cursor,
		const wchar_t *class_name
	);

	void show() const;

	HDC get_device_context() const;

private:
	ILogger &logger_;
	HINSTANCE instance_;
	const wchar_t *class_name_;
	HWND window_;
	HDC device_context_;
};

#endif // HOTLOAD_PLATFORM_WINDOW_SYSTEM_WIN32_WIN32_GL_WINDOW_HH_
