#include "gl_mesh_factory.hh"

#include <cassert>
#include <new>

#include "gl_mesh.hh"

GlMeshFactory::GlMeshFactory(ILogger &logger, IAllocator &allocator)
	: logger_(logger)
	, allocator_(allocator)
{ }

void *GlMeshFactory::create_mesh(
	const Vertex *const vertices,
	const std::size_t vertex_count,
	const std::uint16_t *const indices,
	const std::size_t index_count
)
{
	assert(vertices != nullptr);
	assert(vertex_count > 0);
	assert(indices != nullptr);
	assert(index_count > 0);

	void *const mesh_memory = allocator_.allocate(sizeof(GlMesh));
	if (mesh_memory == nullptr) {
		return nullptr;
	}

	new(mesh_memory) GlMesh(logger_, vertices, vertex_count, indices, index_count);
	return mesh_memory;
}

void GlMeshFactory::destroy_mesh(void *const mesh)
{
	assert(mesh != nullptr);
	assert((reinterpret_cast<std::uintptr_t>(mesh) % alignof(GlMesh)) == 0);

	static_cast<GlMesh *>(mesh)->~GlMesh();
	allocator_.free(mesh);
}
