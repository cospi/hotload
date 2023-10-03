#include "gl_program.hh"

#include <cassert>

static char *get_program_info_log(const GLuint program, ILogger &logger, IAllocator &allocator)
{
	assert(program != 0);

	GLint length = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	if (length <= 0) {
		logger.log(LogLevel::ERR, "Getting program info log length failed.");
		return nullptr;
	}

	void *const log_memory = allocator.allocate(static_cast<std::size_t>(length));
	if (log_memory == nullptr) {
		return nullptr;
	}

	char *const log = static_cast<char *>(log_memory);
	glGetProgramInfoLog(program, length, nullptr, log);
	log[length - 1] = '\0';
	return log;
}

static bool link_program(const GLuint program, ILogger &logger, IAllocator &allocator)
{
	assert(program != 0);

	GLint status = GL_FALSE;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		logger.log(LogLevel::ERR, "Linking OpenGL program (%lu) failed.", program);
		if (char *const info_log = get_program_info_log(program, logger, allocator); info_log != nullptr) {
			logger.log(LogLevel::ERR, info_log);
			allocator.free(info_log);
		}
		return false;
	}
	return true;
}

GlProgram::GlProgram(ILogger &logger)
	: logger_(logger)
	, program_(0)
{ }

GlProgram::~GlProgram()
{
	if (const GLuint program = program_; program != 0) {
		glDeleteProgram(program);
		logger_.log(LogLevel::INFO, "Destroyed OpenGL program (%lu).", program);
	}
}

bool GlProgram::init(const GlShader **const shaders, const std::size_t shader_count, IAllocator &allocator)
{
	assert(program_ == 0);
	assert(shaders != nullptr);
	assert(shader_count > 0);

	ILogger &logger = logger_;

	const GLuint program = glCreateProgram();
	if (program == 0) {
		logger.log(LogLevel::ERR, "Creating OpenGL program failed.");
		return false;
	}

	for (std::size_t i = 0; i < shader_count; ++i) {
		glAttachShader(program, shaders[i]->get_shader());
	}

	const bool success = link_program(program, logger, allocator);

	for (std::size_t i = 0; i < shader_count; ++i) {
		glDetachShader(program, shaders[i]->get_shader());
	}

	if (!success) {
		glDeleteProgram(program);
		return false;
	}

	program_ = program;
	logger.log(LogLevel::INFO, "Created OpenGL program (%lu).", program);
	return true;
}

GLuint GlProgram::get_program() const
{
	assert(program_ != 0);

	return program_;
}
