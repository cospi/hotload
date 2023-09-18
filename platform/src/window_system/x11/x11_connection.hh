#ifndef HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_CONNECTION_HH_
#define HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_CONNECTION_HH_

#include <X11/Xlib.h>

#include <common/log/i_logger.hh>

class X11Connection final {
public:
	explicit X11Connection(ILogger &logger);
	~X11Connection();

	X11Connection(const X11Connection &) = delete;
	X11Connection &operator=(const X11Connection &) = delete;

	bool init(const char *display_name);

	Display *get_display() const;
	int get_default_screen() const;
	Atom get_wm_delete_window() const;

private:
	ILogger &logger_;
	Display *display_;
	int default_screen_;
	Atom wm_delete_window_;
};

#endif // HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_CONNECTION_HH_
