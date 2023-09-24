#ifndef HOTLOAD_PLATFORM_RENDER_GL_GL_MESH_FACTORY_HH_
#define HOTLOAD_PLATFORM_RENDER_GL_GL_MESH_FACTORY_HH_

#include <common/log/i_logger.hh>
#include <common/memory/i_allocator.hh>
#include <common/render/i_mesh_factory.hh>

class GlMeshFactory final : public IMeshFactory {
public:
	GlMeshFactory(ILogger &logger, IAllocator &allocator);
	~GlMeshFactory() override = default;

	GlMeshFactory(const GlMeshFactory &) = delete;
	GlMeshFactory &operator=(const GlMeshFactory &) = delete;

	void *create_mesh(
		const Vertex *vertices,
		std::size_t vertex_count,
		const std::uint16_t *indices,
		std::size_t index_count
	) override;
	void destroy_mesh(void *mesh) override;

private:
	ILogger &logger_;
	IAllocator &allocator_;
};

#endif // HOTLOAD_PLATFORM_RENDER_GL_GL_MESH_FACTORY_HH_
