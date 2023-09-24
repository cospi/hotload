#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_SHADER_PIPELINE_FACTORY_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_SHADER_PIPELINE_FACTORY_HH_

#include <common/file_system/i_file_system.hh>
#include <common/log/i_logger.hh>
#include <common/memory/i_allocator.hh>
#include <common/render/i_shader_pipeline_factory.hh>

class GlShaderPipelineFactory : public IShaderPipelineFactory {
public:
	GlShaderPipelineFactory(ILogger &logger, IAllocator &allocator, IFileSystem &file_system);
	~GlShaderPipelineFactory() override = default;

	GlShaderPipelineFactory(const GlShaderPipelineFactory &) = delete;
	GlShaderPipelineFactory &operator=(const GlShaderPipelineFactory &) = delete;

	void *create_shader_pipeline(const char *filename) override;
	void destroy_shader_pipeline(void *shader_pipeline) override;
	bool try_get_shader_pipeline_uniform(
		void *shader_pipeline,
		const char *uniform_name,
		std::int32_t *out_uniform
	) override;

private:
	ILogger &logger_;
	IAllocator &allocator_;
	IFileSystem &file_system_;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_SHADER_PIPELINE_FACTORY_HH_
