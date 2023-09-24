#ifndef HOTLOAD_COMMON_RENDER_I_SHADER_PIPELINE_FACTORY_HH_
#define HOTLOAD_COMMON_RENDER_I_SHADER_PIPELINE_FACTORY_HH_

#include <cstdint>

class IShaderPipelineFactory {
public:
	virtual ~IShaderPipelineFactory() = default;

	virtual void *create_shader_pipeline(const char *filename) = 0;
	virtual void destroy_shader_pipeline(void *shader_pipeline) = 0;
	virtual bool try_get_shader_pipeline_uniform(
		void *shader_pipeline,
		const char *uniform_name,
		std::int32_t *out_uniform
	) = 0;
};

#endif // HOTLOAD_COMMON_RENDER_I_SHADER_PIPELINE_FACTORY_HH_
