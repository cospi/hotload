#include "win32_gl_extensions.hh"

#include <cassert>

#include "win32_gl_context.hh"
#include "win32_gl_window.hh"
#include "../../render/gl/gl.hh"

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

static const wchar_t DUMMY_WINDOW_TITLE[] = L"HotloadDummy";
static const wchar_t DUMMY_WINDOW_CLASS_NAME[] = L"HotloadDummyWindowClass";
static const int DUMMY_CONTEXT_PIXEL_FORMAT_ATTRIBS[] = {
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
static const int DUMMY_CONTEXT_ATTRIBS[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	WGL_CONTEXT_MINOR_VERSION_ARB, 0,
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	0
};

static HGLRC create_dummy_context(const HDC device_context)
{
	assert(device_context != nullptr);

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

	const int suggested_pixel_format_index = ChoosePixelFormat(device_context, &pixel_format);
	if (suggested_pixel_format_index == 0) {
		return nullptr;
	}

	if (DescribePixelFormat(
		device_context,
		suggested_pixel_format_index,
		sizeof(PIXELFORMATDESCRIPTOR),
		&pixel_format
	) == 0) {
		return nullptr;
	}

	if (SetPixelFormat(device_context, suggested_pixel_format_index, &pixel_format) == FALSE) {
		return nullptr;
	}

	return wglCreateContext(device_context);
}

bool win32_gl_init_context_creation_extensions(HINSTANCE instance, ILogger &logger)
{
	Win32GlWindow dummy_window(logger);
	if (!dummy_window.init(
		0,
		0,
		DUMMY_WINDOW_TITLE,
		CS_OWNDC,
		DefWindowProcW,
		instance,
		nullptr,
		DUMMY_WINDOW_CLASS_NAME
	)) {
		return false;
	}


	const HDC device_context = dummy_window.get_device_context();
	const HGLRC dummy_context = create_dummy_context(device_context);
	if (dummy_context == nullptr) {
		logger.log(LogLevel::ERR, "Creating Win32 OpenGL dummy context failed.");
		return false;
	}

	if (wglMakeCurrent(device_context, dummy_context) == FALSE) {
		logger.log(LogLevel::ERR, "Making context current failed.");
		wglDeleteContext(dummy_context);
		return false;
	}

	PFNWGLCHOOSEPIXELFORMATARBPROC wgl_choose_pixel_format_arb;
	*reinterpret_cast<PROC *>(&wgl_choose_pixel_format_arb) = wglGetProcAddress("wglChoosePixelFormatARB");
	if (wgl_choose_pixel_format_arb == nullptr) {
		logger.log(LogLevel::ERR, "wglChoosePixelFormatARB not found.");
		wglDeleteContext(dummy_context);
		return false;
	}

	PFNWGLCREATECONTEXTATTRIBSARBPROC wgl_create_context_attribs_arb;
	*reinterpret_cast<PROC *>(&wgl_create_context_attribs_arb) = wglGetProcAddress("wglCreateContextAttribsARB");
	if (wgl_create_context_attribs_arb == nullptr) {
		logger.log(LogLevel::ERR, "wglCreateContextAttribsARB not found.");
		wglDeleteContext(dummy_context);
		return false;
	}

	wglChoosePixelFormatARB = wgl_choose_pixel_format_arb;
	wglCreateContextAttribsARB = wgl_create_context_attribs_arb;

	wglMakeCurrent(nullptr, nullptr);

	return true;
}

bool win32_gl_init_extensions(void)
{
	#define INIT_PROC(type, proc) \
		do { \
			*reinterpret_cast<PROC *>(&proc) = wglGetProcAddress(#proc); \
			if (proc == nullptr) { \
				return false; \
			} \
		} while (false)

	INIT_PROC(PFNGLGENBUFFERSPROC, glGenBuffers);
	INIT_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
	INIT_PROC(PFNGLBINDBUFFERPROC, glBindBuffer);
	INIT_PROC(PFNGLBUFFERDATAPROC, glBufferData);
	INIT_PROC(PFNGLMAPBUFFERPROC, glMapBuffer);
	INIT_PROC(PFNGLUNMAPBUFFERPROC, glUnmapBuffer);

	INIT_PROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
	INIT_PROC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
	INIT_PROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);

	INIT_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
	INIT_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);

	INIT_PROC(PFNGLCREATESHADERPROC, glCreateShader);
	INIT_PROC(PFNGLDELETESHADERPROC, glDeleteShader);
	INIT_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
	INIT_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
	INIT_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
	INIT_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);

	INIT_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
	INIT_PROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
	INIT_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
	INIT_PROC(PFNGLDETACHSHADERPROC, glDetachShader);
	INIT_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
	INIT_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
	INIT_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
	INIT_PROC(PFNGLUSEPROGRAMPROC, glUseProgram);

	INIT_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
	INIT_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);

	#undef INIT_PROC

	return true;
}
