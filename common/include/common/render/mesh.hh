#ifndef HOTLOAD_COMMON_RENDER_MESH_HH_
#define HOTLOAD_COMMON_RENDER_MESH_HH_

#include "i_mesh_factory.hh"

class Mesh final {
public:
	explicit Mesh(IMeshFactory &factory);
	~Mesh();

	Mesh(const Mesh &) = delete;
	Mesh &operator=(const Mesh &) = delete;

	bool init(
		const Vertex *vertices,
		std::size_t vertex_count,
		const std::uint16_t *indices,
		std::size_t index_count
	);

	void *get_mesh() const;

private:
	IMeshFactory &factory_;
	void *mesh_;
};

#endif // HOTLOAD_COMMON_RENDER_MESH_HH_
