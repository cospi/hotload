#include "gl_shader.hh"

#include <cassert>

#include <common/file_system/file.hh>
#include <common/memory/allocation.hh>

static char *get_shader_info_log(const GLuint shader, ILogger &logger, IAllocator &allocator)
{
	assert(shader != 0);

	GLint length = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length <= 0) {
		logger.log(LogLevel::ERR, "Getting shader info log length failed.");
		return nullptr;
	}

	void *const log_memory = allocator.allocate(static_cast<std::size_t>(length));
	if (log_memory == nullptr) {
		return nullptr;
	}

	char *const log = static_cast<char *>(log_memory);
	glGetShaderInfoLog(shader, length, nullptr, log);
	log[length - 1] = '\0';
	return log;
}

static bool compile_shader(const GLuint shader, ILogger &logger, IAllocator &allocator)
{
	assert(shader != 0);

	GLint status = GL_FALSE;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		logger.log(LogLevel::ERR, "Compiling OpenGL shader (%lu) failed.", shader);
		if (char *const info_log = get_shader_info_log(shader, logger, allocator); info_log != nullptr) {
			logger.log(LogLevel::ERR, info_log);
			allocator.free(info_log);
		}
		return false;
	}
	return true;
}

GlShader::GlShader(ILogger &logger)
	: logger_(logger)
	, shader_(0)
{ }

GlShader::~GlShader()
{
	if (const GLuint shader = shader_; shader != 0) {
		glDeleteShader(shader);
		logger_.log(LogLevel::INFO, "Destroyed OpenGL shader (%lu).", shader);
	}
}

bool GlShader::init_from_source(const GLenum type, const char *source, IAllocator &allocator)
{
	assert(shader_ == 0);
	assert(source != nullptr);

	ILogger &logger = logger_;

	const GLuint shader = glCreateShader(type);
	if (shader == 0) {
		logger.log(LogLevel::ERR, "Creating OpenGL shader failed.");
		return false;
	}

	glShaderSource(shader, 1, &source, nullptr);
	if (!compile_shader(shader, logger, allocator)) {
		glDeleteShader(shader);
		return false;
	}

	shader_ = shader;
	logger.log(LogLevel::INFO, "Created OpenGL shader (%lu).", shader);
	return true;
}

bool GlShader::init_from_file(
	const GLenum type,
	const char *const path,
	IAllocator &allocator,
	IFileSystem &file_system
)
{
	assert(shader_ == 0);
	assert(path != nullptr);

	File file(file_system);
	if (!file.init(path, "rb")) {
		return false;
	}

	std::size_t size;
	if (!file.try_get_size(&size)) {
		return false;
	}

	Allocation source_allocation(allocator);
	if (!source_allocation.init(size + 1)) {
		return false;
	}

	char *const source = static_cast<char *>(source_allocation.get_memory());
	if (!file.try_read(size, reinterpret_cast<unsigned char *>(source))) {
		return false;
	}
	source[size] = '\0';

	return init_from_source(type, source, allocator);
}

GLuint GlShader::get_shader() const
{
	assert(shader_ != 0);

	return shader_;
}
