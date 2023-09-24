#include <common/render/shader_pipeline.hh>

#include <cassert>

ShaderPipeline::ShaderPipeline(IShaderPipelineFactory &factory)
	: factory_(factory)
	, shader_pipeline_(nullptr)
{ }

ShaderPipeline::~ShaderPipeline()
{
	if (void *const shader_pipeline = shader_pipeline_; shader_pipeline != nullptr) {
		factory_.destroy_shader_pipeline(shader_pipeline);
	}
}

bool ShaderPipeline::init(const char *const filename)
{
	assert(shader_pipeline_ == nullptr);
	assert(filename != nullptr);

	void *const shader_pipeline = factory_.create_shader_pipeline(filename);
	if (shader_pipeline == nullptr) {
		return false;
	}

	shader_pipeline_ = shader_pipeline;
	return true;
}

bool ShaderPipeline::try_get_uniform(const char *const uniform_name, std::int32_t *const out_uniform) const
{
	assert(shader_pipeline_ != nullptr);
	assert(uniform_name != nullptr);
	assert(out_uniform != nullptr);

	return factory_.try_get_shader_pipeline_uniform(shader_pipeline_, uniform_name, out_uniform);
}

void *ShaderPipeline::get_shader_pipeline() const
{
	assert(shader_pipeline_ != nullptr);

	return shader_pipeline_;
}
