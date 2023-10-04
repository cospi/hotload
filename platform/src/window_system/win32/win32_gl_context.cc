#include "win32_gl_context.hh"

#include <cassert>
#include <cinttypes>

#include "win32_gl_extensions.hh"
#include "../../render/gl/gl.hh"

static const int PIXEL_FORMAT_ATTRIBS[] = {
	WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
	WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
	WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
	WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
	WGL_COLOR_BITS_ARB, 32,
	WGL_DEPTH_BITS_ARB, 24,
	WGL_ALPHA_BITS_ARB, 8,
	0
};

static const int CONTEXT_ATTRIBS[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	WGL_CONTEXT_MINOR_VERSION_ARB, 0,
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	0
};

Win32GlContext::Win32GlContext(ILogger &logger)
	: logger_(logger)
	, context_(nullptr)
{ }

Win32GlContext::~Win32GlContext()
{
	if (const HGLRC context = context_; context != nullptr) {
		wglDeleteContext(context);
		logger_.log(
			LogLevel::INFO,
			"Destroyed Win32 OpenGL context (%" PRIxPTR ").",
			reinterpret_cast<std::uintptr_t>(static_cast<void *>(context))
		);
	}
}

bool Win32GlContext::init(const HDC device_context)
{
	assert(context_ == nullptr);
	assert(device_context != nullptr);

	int suggested_pixel_format_index;
	UINT pixel_format_count;
	if (wglChoosePixelFormatARB(
		device_context,
		PIXEL_FORMAT_ATTRIBS,
		nullptr,
		1,
		&suggested_pixel_format_index,
		&pixel_format_count
	) == FALSE) {
		logger_.log(LogLevel::ERR, "Choosing pixel format failed.");
		return false;
	}

	PIXELFORMATDESCRIPTOR pixel_format;
	pixel_format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixel_format.nVersion = 1;
	pixel_format.dwFlags = PFD_SUPPORT_OPENGL;
	pixel_format.iPixelType = PFD_TYPE_RGBA;
	pixel_format.cColorBits = 32;
	pixel_format.cRedBits = 0;
	pixel_format.cRedShift = 0;
	pixel_format.cGreenBits = 0;
	pixel_format.cGreenShift = 0;
	pixel_format.cBlueBits = 0;
	pixel_format.cBlueShift = 0;
	pixel_format.cAlphaBits = 8;
	pixel_format.cAlphaShift = 0;
	pixel_format.cAccumBits = 0;
	pixel_format.cAccumRedBits = 0;
	pixel_format.cAccumGreenBits = 0;
	pixel_format.cAccumBlueBits = 0;
	pixel_format.cAccumAlphaBits = 0;
	pixel_format.cDepthBits = 24;
	pixel_format.cStencilBits = 0;
	pixel_format.cAuxBuffers = 0;
	pixel_format.iLayerType = 0;
	pixel_format.bReserved = 0;
	pixel_format.dwLayerMask = 0;
	pixel_format.dwVisibleMask = 0;
	pixel_format.dwDamageMask = 0;

	if (DescribePixelFormat(
		device_context,
		suggested_pixel_format_index,
		sizeof(PIXELFORMATDESCRIPTOR),
		&pixel_format
	) == 0) {
		logger_.log(LogLevel::ERR, "Describing pixel format failed.");
		return false;
	}

	if (SetPixelFormat(device_context, suggested_pixel_format_index, &pixel_format) == FALSE) {
		logger_.log(LogLevel::ERR, "Setting pixel format failed.");
		return false;
	}

	const HGLRC context = wglCreateContextAttribsARB(device_context, nullptr, CONTEXT_ATTRIBS);
	if (context == nullptr) {
		logger_.log(LogLevel::ERR, "Creating Win32 OpenGL context failed.");
		return false;
	}

	context_ = context;
	logger_.log(
		LogLevel::INFO,
		"Created Win32 OpenGL context (%" PRIxPTR ").",
		reinterpret_cast<std::uintptr_t>(static_cast<void *>(context))
	);
	return true;
}

HGLRC Win32GlContext::get_context() const
{
	assert(context_ != nullptr);

	return context_;
}
