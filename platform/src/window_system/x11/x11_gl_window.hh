#ifndef HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_GL_WINDOW_HH_
#define HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_GL_WINDOW_HH_

#include <GL/glx.h>

#include "x11_connection.hh"

class X11GlContext;

class X11GlWindow final {
public:
	X11GlWindow(const X11Connection &connection, ILogger &logger);
	~X11GlWindow();

	X11GlWindow(const X11GlWindow &) = delete;
	X11GlWindow &operator=(const X11GlWindow &) = delete;

	bool init(unsigned int width, unsigned int height, const char *title);

	void make_context_current(const X11GlContext *context) const;
	void swap_buffers() const;

	const X11Connection &get_connection() const;
	GLXFBConfig get_fb_config() const;

private:
	const X11Connection &connection_;
	ILogger &logger_;
	GLXFBConfig fb_config_;
	Colormap colormap_;
	Window window_;
};

#endif // HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_GL_WINDOW_HH_
