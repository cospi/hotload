#include "x11_gl_context.hh"

#include <cassert>
#include <cinttypes>

#include "x11_gl_extensions.hh"

static const int CONTEXT_ATTRIBS[] = {
	GLX_CONTEXT_MAJOR_VERSION_ARB, 2,
	GLX_CONTEXT_MINOR_VERSION_ARB, 0,
	GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
	None
};

static bool s_error = false;

static int signal_error(Display *const display, XErrorEvent *const error_event)
{
	static_cast<void>(display);
	static_cast<void>(error_event);

	s_error = true;
	return 0;
}

X11GlContext::X11GlContext(const X11GlWindow &window, ILogger &logger)
	: window_(window)
	, logger_(logger)
	, context_(nullptr)
{ }

X11GlContext::~X11GlContext()
{
	if (const GLXContext context = context_; context != nullptr) {
		glXDestroyContext(window_.get_connection().get_display(), context);
		logger_.log(
			LogLevel::INFO,
			"Destroyed X OpenGL context (%" PRIxPTR ").",
			reinterpret_cast<std::uintptr_t>(static_cast<void *>(context))
		);
	}
}

bool X11GlContext::init()
{
	assert(context_ == nullptr);

	const PFNGLXCREATECONTEXTATTRIBSARBPROC glx_create_context_attribs_arb =
		reinterpret_cast<PFNGLXCREATECONTEXTATTRIBSARBPROC>(
			glXGetProcAddressARB(reinterpret_cast<const GLubyte *>("glXCreateContextAttribsARB"))
		);
	if (glx_create_context_attribs_arb == nullptr) {
		logger_.log(LogLevel::ERROR, "glXCreateContextAttribsARB not found.");
		return false;
	}

	const X11GlWindow &window = window_;
	const X11Connection &connection = window.get_connection();
	Display *const display = connection.get_display();
	const int screen = connection.get_default_screen();
	const char *const extensions = glXQueryExtensionsString(display, screen);
	if (!x11_gl_has_extension(extensions, "GLX_ARB_create_context")) {
		logger_.log(LogLevel::ERROR, "GLX_ARB_create_context not found.");
		return false;
	}

	s_error = false;
	int (*const previous_error_handler)(Display *, XErrorEvent *) = XSetErrorHandler(signal_error);
	const GLXContext context =
		glx_create_context_attribs_arb(display, window.get_fb_config(), nullptr, True, CONTEXT_ATTRIBS);
	XSync(display, False);
	XSetErrorHandler(previous_error_handler);
	if ((context == nullptr) || s_error) {
		s_error = false;
		if (context != nullptr) {
			glXDestroyContext(display, context);
		}
		logger_.log(LogLevel::ERROR, "Creating X OpenGL context failed.");
		return false;
	}

	context_ = context;
	logger_.log(
		LogLevel::INFO,
		"Created X OpenGL context (%" PRIxPTR ").",
		reinterpret_cast<std::uintptr_t>(static_cast<void *>(context))
	);
	return true;
}

GLXContext X11GlContext::get_context() const
{
	assert(context_ != nullptr);

	return context_;
}
