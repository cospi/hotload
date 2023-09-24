#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_SHADER_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_SHADER_HH_

#include <common/file_system/i_file_system.hh>
#include <common/log/i_logger.hh>
#include <common/memory/i_allocator.hh>

#include "gl.hh"

class GlShader final {
public:
	explicit GlShader(ILogger &logger);
	~GlShader();

	GlShader(const GlShader &) = delete;
	GlShader &operator=(const GlShader &) = delete;

	bool init_from_source(GLenum type, const char *source, IAllocator &allocator);
	bool init_from_file(GLenum type, const char *path, IAllocator &allocator, IFileSystem &file_system);

	GLuint get_shader() const;

private:
	ILogger &logger_;
	GLuint shader_;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_SHADER_HH_
