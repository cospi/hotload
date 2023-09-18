#ifndef HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_GL_CONTEXT_HH_
#define HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_GL_CONTEXT_HH_

#include "x11_gl_window.hh"

class X11GlContext final {
public:
	X11GlContext(const X11GlWindow &window, ILogger &logger);
	~X11GlContext();

	X11GlContext(const X11GlContext &) = delete;
	X11GlContext &operator=(const X11GlContext &) = delete;

	bool init();

	GLXContext get_context() const;

private:
	const X11GlWindow &window_;
	ILogger &logger_;
	GLXContext context_;
};

#endif // HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_GL_CONTEXT_HH_
