#ifndef HOTLOAD_COMMON_RENDER_I_MESH_FACTORY_HH_
#define HOTLOAD_COMMON_RENDER_I_MESH_FACTORY_HH_

#include <cstdint>

#include "vertex.hh"

class IMeshFactory {
public:
	virtual ~IMeshFactory() = default;

	virtual void *create_mesh(
		const Vertex *vertices,
		std::size_t vertex_count,
		const std::uint16_t *indices,
		std::size_t index_count
	) = 0;
	virtual void destroy_mesh(void *mesh) = 0;
};

#endif // HOTLOAD_COMMON_RENDER_I_MESH_FACTORY_HH_
