#include "x11_connection.hh"

#include <cassert>
#include <cinttypes>

X11Connection::X11Connection(ILogger &logger)
	: logger_(logger)
	, display_(nullptr)
	, default_screen_(0)
	, wm_delete_window_(None)
{ }

X11Connection::~X11Connection()
{
	if (Display *const display = display_; display != nullptr) {
		XCloseDisplay(display);
		logger_.log(
			LogLevel::INFO,
			"Closed connection to X server (%" PRIxPTR ").",
			reinterpret_cast<std::uintptr_t>(static_cast<void *>(display))
		);
	}
}

bool X11Connection::init(const char *const display_name)
{
	assert(display_ == nullptr);

	Display *const display = XOpenDisplay(display_name);
	if (display == nullptr) {
		logger_.log(LogLevel::ERROR, "Connecting to X server failed.");
		return false;
	}

	display_ = display;
	default_screen_ = DefaultScreen(display);
	wm_delete_window_ = XInternAtom(display, "WM_DELETE_WINDOW", False);
	logger_.log(
		LogLevel::INFO,
		"Opened connection to X server (%" PRIxPTR ").",
		reinterpret_cast<std::uintptr_t>(static_cast<void *>(display))
	);
	return true;
}

Display *X11Connection::get_display() const
{
	assert(display_ != nullptr);

	return display_;
}

int X11Connection::get_default_screen() const
{
	assert(display_ != nullptr);

	return default_screen_;
}

Atom X11Connection::get_wm_delete_window() const
{
	assert(display_ != nullptr);

	return wm_delete_window_;
}
