#include "x11_gl_extensions.hh"

#include <cassert>
#include <cstring>

#include <GL/glx.h>

#include "../../render/gl/gl.hh"

bool x11_gl_has_extension(const char *const extensions, const char *const extension)
{
	assert(extensions != nullptr);
	assert(extension != nullptr);
	const std::size_t extension_length = std::strlen(extension);
	assert(extension_length > 0);
	assert(std::strchr(extension, ' ') == nullptr);

	if (*extensions == '\0') {
		return false;
	}

	for (const char *start = extensions;;) {
		const char *const found = std::strstr(start, extension);
		if (found == nullptr) {
			break;
		}

		const char *const remaining = found + extension_length;
		if ((found == start) || (found[-1] == ' ')) {
			const char terminator = *remaining;
			if ((terminator == '\0') || (terminator == ' ')) {
				return true;
			}
		}

		start = remaining;
	}

	return false;
}

bool x11_gl_init_extensions()
{
	#define INIT_PROC(type, proc) \
		do { \
			proc = reinterpret_cast<type>( \
				glXGetProcAddressARB(reinterpret_cast<const GLubyte *>(#proc)) \
			); \
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
