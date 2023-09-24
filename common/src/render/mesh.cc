#include <common/render/mesh.hh>

#include <cassert>

Mesh::Mesh(IMeshFactory &factory)
	: factory_(factory)
	, mesh_(nullptr)
{ }

Mesh::~Mesh()
{
	if (void *const mesh = mesh_; mesh != nullptr) {
		factory_.destroy_mesh(mesh);
	}
}

bool Mesh::init(
	const Vertex *const vertices,
	const std::size_t vertex_count,
	const std::uint16_t *const indices,
	const std::size_t index_count
)
{
	assert(mesh_ == nullptr);
	assert(vertices != nullptr);
	assert(vertex_count > 0);
	assert(indices != nullptr);
	assert(index_count > 0);

	void *const mesh = factory_.create_mesh(vertices, vertex_count, indices, index_count);
	if (mesh == nullptr) {
		return false;
	}

	mesh_ = mesh;
	return true;
}

void *Mesh::get_mesh() const
{
	assert(mesh_ != nullptr);

	return mesh_;
}
