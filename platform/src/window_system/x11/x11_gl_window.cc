#include "x11_gl_window.hh"

#include <cassert>

#include "x11_gl_context.hh"

// \note FBConfigs require GLX 1.3 or later.
static const int GLX_MAJOR_MIN = 1;
static const int GLX_MINOR_MIN = 3;

static const int VISUAL_ATTRIBS[] = {
	GLX_X_RENDERABLE, True,
	GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
	GLX_RENDER_TYPE, GLX_RGBA_BIT,
	GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
	GLX_RED_SIZE, 8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE, 8,
	GLX_ALPHA_SIZE, 8,
	GLX_DEPTH_SIZE, 24,
	GLX_DOUBLEBUFFER, True,
	None
};

static bool has_required_glx_version(Display *const display)
{
	assert(display != nullptr);

	int major, minor;
	return
		(glXQueryVersion(display, &major, &minor) == True)
		&& (
			(major > GLX_MAJOR_MIN)
			|| ((major == GLX_MAJOR_MIN) && (minor >= GLX_MINOR_MIN))
		);
}

static GLXFBConfig find_fb_config(Display *const display, const int screen)
{
	assert(display != nullptr);

	int count;
	GLXFBConfig *const fb_configs = glXChooseFBConfig(display, screen, VISUAL_ATTRIBS, &count);
	if (fb_configs == nullptr) {
		return nullptr;
	}

	const GLXFBConfig fb_config = fb_configs[0];
	XFree(fb_configs);
	return fb_config;
}

X11GlWindow::X11GlWindow(const X11Connection &connection, ILogger &logger)
	: connection_(connection)
	, logger_(logger)
	, fb_config_(nullptr)
	, colormap_(None)
	, window_(None)
{ }

X11GlWindow::~X11GlWindow()
{
	if (const GLXFBConfig fb_config = fb_config_; fb_config != nullptr) {
		Display *const display = connection_.get_display();
		const Window window = window_;
		XDestroyWindow(display, window);
		XFreeColormap(display, colormap_);
		logger_.log(LogLevel::INFO, "Destroyed X OpenGL window (%lu).", window);
	}
}

bool X11GlWindow::init(const unsigned int width, const unsigned int height, const char *const title)
{
	assert(fb_config_ == nullptr);
	assert(width > 0);
	assert(height > 0);
	assert(title != nullptr);

	const X11Connection &connection = connection_;
	Display *const display = connection.get_display();
	if (!has_required_glx_version(display)) {
		logger_.log(
			LogLevel::ERROR,
			"Required GLX version (minimum %d.%d) not supported.",
			GLX_MAJOR_MIN,
			GLX_MINOR_MIN
		);
		return false;
	}

	const int screen = connection.get_default_screen();
	const GLXFBConfig fb_config = find_fb_config(display, screen);
	if (fb_config == nullptr) {
		logger_.log(LogLevel::ERROR, "Getting framebuffer configuration failed.");
		return false;
	}

	XVisualInfo *const visual_info = glXGetVisualFromFBConfig(display, fb_config);
	Visual *const visual = visual_info->visual;

	const Window root_window = RootWindow(display, screen);
	const Colormap colormap = XCreateColormap(display, root_window, visual, AllocNone);

	XSetWindowAttributes window_attribs;
	window_attribs.background_pixmap = None;
	window_attribs.border_pixel = 0;
	window_attribs.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask;
	window_attribs.colormap = colormap;

	const Window window = XCreateWindow(
		display,
		root_window,
		0, // X
		0, // Y
		width,
		height,
		0, // Border width
		visual_info->depth,
		InputOutput,
		visual,
		CWBorderPixel | CWColormap | CWEventMask,
		&window_attribs
	);

	XFree(visual_info);

	XStoreName(display, window, title);
	XMapWindow(display, window);

	Atom wm_delete_window = connection.get_wm_delete_window();
	XSetWMProtocols(display, window, &wm_delete_window, 1);

	fb_config_ = fb_config;
	colormap_ = colormap;
	window_ = window;
	logger_.log(LogLevel::INFO, "Created X OpenGL window (%lu).", window);
	return true;
}

void X11GlWindow::make_context_current(const X11GlContext *const context) const
{
	assert(fb_config_ != nullptr);

	Display *const display = connection_.get_display();
	if (context != nullptr) {
		glXMakeCurrent(display, window_, context->get_context());
	} else {
		glXMakeCurrent(display, None, nullptr);
	}
}

void X11GlWindow::swap_buffers() const
{
	assert(fb_config_ != nullptr);

	glXSwapBuffers(connection_.get_display(), window_);
}

const X11Connection &X11GlWindow::get_connection() const
{
	assert(fb_config_ != nullptr);

	return connection_;
}

GLXFBConfig X11GlWindow::get_fb_config() const
{
	assert(fb_config_ != nullptr);

	return fb_config_;
}
