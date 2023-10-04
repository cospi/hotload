#include "win32_gl_window.hh"

#include <cassert>
#include <cinttypes>
#include <limits>

Win32GlWindow::Win32GlWindow(ILogger &logger)
	: logger_(logger)
	, instance_(nullptr)
	, class_name_(nullptr)
	, window_(nullptr)
	, device_context_(nullptr)
{ }

Win32GlWindow::~Win32GlWindow()
{
	if (const HDC device_context = device_context_; device_context != nullptr) {
		ReleaseDC(window_, device_context_);
		const HWND window = window_;
		DestroyWindow(window);
		UnregisterClassW(class_name_, instance_);
		logger_.log(
			LogLevel::INFO,
			"Destroyed Win32 OpenGL window (%" PRIxPTR ").",
			reinterpret_cast<std::uintptr_t>(static_cast<void *>(window))
		);
	}
}

bool Win32GlWindow::init(
	const unsigned int width,
	const unsigned int height,
	const wchar_t *const title,
	const UINT style,
	const WNDPROC proc,
	const HINSTANCE instance,
	const HCURSOR cursor,
	const wchar_t *const class_name
)
{
	assert(width > 0);
	assert(width <= std::numeric_limits<int>::max());
	assert(height > 0);
	assert(height <= std::numeric_limits<int>::max());
	assert(title != nullptr);
	assert(instance != nullptr);
	assert(class_name != nullptr);

	WNDCLASSEXW window_class;
	window_class.cbSize = sizeof(WNDCLASSEXW);
	window_class.style = style;
	window_class.lpfnWndProc = proc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance;
	window_class.hIcon = nullptr;
	window_class.hCursor = cursor;
	window_class.hbrBackground = nullptr;
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = class_name;
	window_class.hIconSm = nullptr;

	if (RegisterClassExW(&window_class) == FALSE) {
		logger_.log(LogLevel::ERR, "Registering window class failed.");
		return false;
	}

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = static_cast<int>(width);
	rect.bottom = static_cast<int>(height);
	if (AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE) == FALSE) {
		logger_.log(LogLevel::ERR, "Adjusting window rect failed.");
		UnregisterClassW(class_name, instance);
		return false;
	}

	const HWND window = CreateWindowExW(
		0,
		class_name,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		instance,
		nullptr
	);
	if (window == nullptr) {
		logger_.log(LogLevel::ERR, "Creating window failed.");
		UnregisterClassW(class_name, instance);
		return false;
	}

	const HDC device_context = GetDC(window);
	if (device_context == nullptr) {
		logger_.log(LogLevel::ERR, "Getting device context failed.");
		DestroyWindow(window);
		UnregisterClassW(class_name, instance);
		return -1;
	}

	instance_ = instance;
	class_name_ = class_name;
	window_ = window;
	device_context_ = device_context;
	logger_.log(
		LogLevel::INFO,
		"Created Win32 OpenGL window (%" PRIxPTR ").",
		reinterpret_cast<std::uintptr_t>(static_cast<void *>(window))
	);
	return true;
}

void Win32GlWindow::show() const
{
	assert(device_context_ != nullptr);

	ShowWindow(window_, SW_SHOW);
}

HDC Win32GlWindow::get_device_context() const
{
	assert(device_context_ != nullptr);

	return device_context_;
}
