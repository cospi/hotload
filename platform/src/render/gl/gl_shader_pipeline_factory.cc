#include "gl_shader_pipeline_factory.hh"

#include <cassert>
#include <cinttypes>
#include <new>

#include <common/file_system/path.hh>

#include "gl_program.hh"

static const char *get_shader_extension(const GLenum type)
{
	switch (type) {
	case GL_VERTEX_SHADER:
		return ".vert";
	case GL_FRAGMENT_SHADER:
		return ".frag";
	default:
		assert(false);
		return "";
	}
}

static bool init_shader_path(Path &path, const GLenum type, const char *const filename)
{
	assert(filename != nullptr);

	return path.init("res/shaders/gl/", filename, get_shader_extension(type));
}

static bool init_shader(
	GlShader &shader,
	const GLenum type,
	const char *const filename,
	IAllocator &allocator,
	IFileSystem &file_system
)
{
	assert(filename != nullptr);

	Path path(allocator);
	return
		init_shader_path(path, type, filename)
		&& shader.init_from_file(type, path.get_path(), allocator, file_system);
}

GlShaderPipelineFactory::GlShaderPipelineFactory(ILogger &logger, IAllocator &allocator, IFileSystem &file_system)
	: logger_(logger)
	, allocator_(allocator)
	, file_system_(file_system)
{ }

void *GlShaderPipelineFactory::create_shader_pipeline(const char *const filename)
{
	assert(filename != nullptr);

	ILogger &logger = logger_;
	IAllocator &allocator = allocator_;
	IFileSystem &file_system = file_system_;

	GlShader vertex_shader(logger);
	if (!init_shader(vertex_shader, GL_VERTEX_SHADER, filename, allocator, file_system)) {
		return nullptr;
	}

	GlShader fragment_shader(logger);
	if (!init_shader(fragment_shader, GL_FRAGMENT_SHADER, filename, allocator, file_system)) {
		return nullptr;
	}

	void *const program_memory  = allocator.allocate(sizeof(GlProgram));
	if (program_memory == nullptr) {
		return nullptr;
	}

	const GlShader *shaders[2] = { &vertex_shader, &fragment_shader };
	GlProgram *const program = new(program_memory) GlProgram(logger);
	if (!program->init(shaders, 2, allocator)) {
		program->~GlProgram();
		allocator.free(program_memory);
		return nullptr;
	}

	return program_memory;
}

void GlShaderPipelineFactory::destroy_shader_pipeline(void *const shader_pipeline)
{
	assert(shader_pipeline != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(shader_pipeline) % alignof(GlProgram)) == 0);

	static_cast<GlProgram *>(shader_pipeline)->~GlProgram();
	allocator_.free(shader_pipeline);
}


bool GlShaderPipelineFactory::try_get_shader_pipeline_uniform(
	void *const shader_pipeline,
	const char *const uniform_name,
	std::int32_t *const out_uniform
)
{
	assert(shader_pipeline != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(shader_pipeline) % alignof(GlProgram)) == 0);
	assert(uniform_name != nullptr);
	assert(out_uniform != nullptr);

	const GLint uniform =
		glGetUniformLocation(static_cast<GlProgram *>(shader_pipeline)->get_program(), uniform_name);
	if (uniform == -1) {
		logger_.log(LogLevel::ERROR, "Getting uniform \"%s\" failed.", uniform_name);
		return false;
	}

	*out_uniform = uniform;
	return true;
}
