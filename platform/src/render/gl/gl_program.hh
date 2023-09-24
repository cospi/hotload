#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_PROGRAM_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_PROGRAM_HH_

#include "gl_shader.hh"

class GlProgram final {
public:
	explicit GlProgram(ILogger &logger);
	~GlProgram();

	GlProgram(const GlProgram &) = delete;
	GlProgram &operator=(const GlProgram &) = delete;

	bool init(const GlShader **shaders, std::size_t shader_count, IAllocator &allocator);

	GLuint get_program() const;

private:
	ILogger &logger_;
	GLuint program_;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_PROGRAM_HH_
