#ifndef HOTLOAD_COMMON_RENDER_GEOMETRY_HH_
#define HOTLOAD_COMMON_RENDER_GEOMETRY_HH_

#include <cstdint>

#include "vertex.hh"

static const int CUBE_VERTEX_COUNT = 24;
static const int CUBE_INDEX_COUNT = 36;

void geometry_init_cube(Vertex *vertices, std::uint16_t *indices, float extents);

#endif // HOTLOAD_COMMON_RENDER_GEOMETRY_HH_
