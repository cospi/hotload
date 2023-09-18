#ifndef HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_WINDOW_HH_
#define HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_WINDOW_HH_

#include "x11_connection.hh"

Window x11_window_create(
	const X11Connection &connection,
	ILogger &logger,
	unsigned int width,
	unsigned int height,
	const char *title
);

#endif // HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_WINDOW_HH_
