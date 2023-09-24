#ifndef HOTLOAD_COMMON_RENDER_SHADER_PIPELINE_HH_
#define HOTLOAD_COMMON_RENDER_SHADER_PIPELINE_HH_

#include "i_shader_pipeline_factory.hh"

class ShaderPipeline final {
public:
	explicit ShaderPipeline(IShaderPipelineFactory &factory);
	~ShaderPipeline();

	ShaderPipeline(const ShaderPipeline &) = delete;
	ShaderPipeline &operator=(const ShaderPipeline &) = delete;

	bool init(const char *filename);

	bool try_get_uniform(const char *uniform_name, std::int32_t *out_uniform) const;

	void *get_shader_pipeline() const;

private:
	IShaderPipelineFactory &factory_;
	void *shader_pipeline_;
};

#endif // HOTLOAD_COMMON_RENDER_SHADER_PIPELINE_HH_
