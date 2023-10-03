#include "win32_gl_extensions.hh"

#include <cassert>

#include "../../render/gl/gl.hh"

#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC, const int *, const float *, UINT, int *, UINT *);
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int *);

static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

bool win32_gl_init_context_creation_extensions(HINSTANCE instance)
{
	wchar_t window_class_name[] = L"ShaderTestbedWGLExtensionsWindowClass";
	WNDCLASSEXW window_class;
	window_class.cbSize = sizeof(WNDCLASSEXW);
	window_class.style = CS_OWNDC;
	window_class.lpfnWndProc = DefWindowProcW;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance;
	window_class.hIcon = nullptr;
	window_class.hCursor = nullptr;
	window_class.hbrBackground = nullptr;
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = window_class_name;
	window_class.hIconSm = nullptr;

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

	bool success = false;
	HWND window;
	HDC device_context;
	int suggested_pixel_format_index;
	HGLRC rendering_context;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wgl_create_context_attribs_arb;
	PFNWGLCHOOSEPIXELFORMATARBPROC wgl_choose_pixel_format_arb;

	if (RegisterClassExW(&window_class) == 0) {
		return false;
	}

	window = CreateWindowExW(
		0,
		window_class_name,
		L"Shader Testbed",
		0,
		0,
		0,
		0,
		0,
		nullptr,
		nullptr,
		instance,
		nullptr
	);
	if (window == nullptr) {
		goto out_unregister_window_class;
	}

	device_context = GetDC(window);
	if (device_context == nullptr) {
		goto out_destroy_window;
	}

	suggested_pixel_format_index = ChoosePixelFormat(device_context, &pixel_format);
	if (suggested_pixel_format_index == 0) {
		goto out_release_device_context;
	}

	if (DescribePixelFormat(
		device_context,
		suggested_pixel_format_index,
		sizeof(PIXELFORMATDESCRIPTOR),
		&pixel_format
	) == 0) {
		goto out_release_device_context;
	}

	if (SetPixelFormat(device_context, suggested_pixel_format_index, &pixel_format) == FALSE) {
		goto out_release_device_context;
	}

	rendering_context = wglCreateContext(device_context);
	if (rendering_context == nullptr) {
		goto out_release_device_context;
	}

	if (wglMakeCurrent(device_context, rendering_context) == FALSE) {
		goto out_delete_rendering_context;
	}

	*reinterpret_cast<PROC *>(&wgl_choose_pixel_format_arb) = wglGetProcAddress("wglChoosePixelFormatARB");
	if (wgl_choose_pixel_format_arb == nullptr) {
		goto out_reset_current_rendering_context;
	}

	*reinterpret_cast<PROC *>(&wgl_create_context_attribs_arb) = wglGetProcAddress("wglCreateContextAttribsARB");
	if (wgl_create_context_attribs_arb == nullptr) {
		goto out_reset_current_rendering_context;
	}

	wglChoosePixelFormatARB = wgl_choose_pixel_format_arb;
	wglCreateContextAttribsARB = wgl_create_context_attribs_arb;
	success = true;

out_reset_current_rendering_context:
	wglMakeCurrent(nullptr, nullptr);
out_delete_rendering_context:
	wglDeleteContext(rendering_context);
out_release_device_context:
	ReleaseDC(window, device_context);
out_destroy_window:
	DestroyWindow(window);
out_unregister_window_class:
	UnregisterClassW(window_class_name, instance);
	return success;
}

HGLRC win32_gl_create_context(HDC device_context)
{
	int pixel_format_attribs[] = {
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
	int suggested_pixel_format_index;
	UINT pixel_format_count;
	if (wglChoosePixelFormatARB(
		device_context,
		pixel_format_attribs,
		nullptr,
		1,
		&suggested_pixel_format_index,
		&pixel_format_count
	) == FALSE) {
		return nullptr;
	}

	PIXELFORMATDESCRIPTOR pixel_format;
	pixel_format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixel_format.nVersion = 1;
	pixel_format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
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
		return nullptr;
	}

	if (SetPixelFormat(device_context, suggested_pixel_format_index, &pixel_format) == FALSE) {
		return nullptr;
	}

	int context_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
	return wglCreateContextAttribsARB(device_context, nullptr, context_attribs);
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
