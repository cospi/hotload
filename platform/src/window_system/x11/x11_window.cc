#include "x11_window.hh"

#include <cassert>

Window x11_window_create(
	const X11Connection &connection,
	ILogger &logger,
	const unsigned int width,
	const unsigned int height,
	const char *const title
)
{
	assert(width > 0);
	assert(height > 0);
	assert(title != nullptr);

	Display *const display = connection.get_display();
	const int screen = connection.get_default_screen();
	const Window window = XCreateSimpleWindow(
		display,
		RootWindow(display, screen),
		0, // X
		0, // Y
		width,
		height,
		0, // Border width
		0, // Border color
		BlackPixel(display, screen) // Background color
	);
	XStoreName(display, window, title);
	XMapWindow(display, window);

	Atom wm_delete_window = connection.get_wm_delete_window();
	XSetWMProtocols(display, window, &wm_delete_window, 1);

	logger.log(LogLevel::INFO, "Created X window (%lu).", window);
	return window;
}
