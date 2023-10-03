#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_HH_

#include <cstddef>

#include <GL/gl.h>

typedef std::ptrdiff_t GLsizeiptr;
typedef char GLchar;

#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_WRITE_ONLY 0x88B9
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84

typedef void (APIENTRY * PFNGLGENBUFFERSPROC)(GLsizei, GLuint *);
typedef void (APIENTRY * PFNGLDELETEBUFFERSPROC)(GLsizei, const GLuint *);
typedef void (APIENTRY * PFNGLBINDBUFFERPROC)(GLenum, GLuint);
typedef void (APIENTRY * PFNGLBUFFERDATAPROC)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
typedef void *(APIENTRY * PFNGLMAPBUFFERPROC)(GLenum, GLenum);
typedef void (APIENTRY * PFNGLUNMAPBUFFERPROC)(GLenum);

typedef void (APIENTRY * PFNGLGENVERTEXARRAYSPROC)(GLsizei, GLuint *);
typedef void (APIENTRY * PFNGLDELETEVERTEXARRAYSPROC)(GLsizei, GLuint *);
typedef void (APIENTRY * PFNGLBINDVERTEXARRAYPROC)(GLuint);

typedef void (APIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint);
typedef void (APIENTRY * PFNGLVERTEXATTRIBPOINTERPROC)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);

typedef GLuint (APIENTRY * PFNGLCREATESHADERPROC)(GLenum);
typedef void (APIENTRY * PFNGLDELETESHADERPROC)(GLuint);
typedef void (APIENTRY * PFNGLSHADERSOURCEPROC)(GLuint, GLsizei, const GLchar **, const GLint *);
typedef void (APIENTRY * PFNGLCOMPILESHADERPROC)(GLuint);
typedef void (APIENTRY * PFNGLGETSHADERINFOLOGPROC)(GLuint, GLsizei, GLsizei *, GLchar *);
typedef void (APIENTRY * PFNGLGETSHADERIVPROC)(GLuint, GLenum, GLint *);

typedef GLuint (APIENTRY * PFNGLCREATEPROGRAMPROC)(void);
typedef void (APIENTRY * PFNGLDELETEPROGRAMPROC)(GLuint);
typedef void (APIENTRY * PFNGLATTACHSHADERPROC)(GLuint, GLuint);
typedef void (APIENTRY * PFNGLDETACHSHADERPROC)(GLuint, GLuint);
typedef void (APIENTRY * PFNGLLINKPROGRAMPROC)(GLuint);
typedef void (APIENTRY * PFNGLGETPROGRAMIVPROC)(GLuint, GLenum, GLint *);
typedef void (APIENTRY * PFNGLGETPROGRAMINFOLOGPROC)(GLuint, GLsizei, GLsizei *, GLchar *);
typedef void (APIENTRY * PFNGLUSEPROGRAMPROC)(GLuint);

typedef GLint (APIENTRY * PFNGLGETUNIFORMLOCATIONPROC)(GLuint, const GLchar *);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX4FVPROC)(GLint, GLsizei, GLboolean, const GLfloat *);

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;

extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC glUseProgram;

extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_HH_
