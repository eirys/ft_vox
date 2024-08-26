#pragma once

#include <vector>

#include "vertex.h"

namespace vox::gfx {

struct WorldGfx final {

    std::vector<VertexInstance> m_instances;

    // Portion of world generated offset. At first, center of full world.
    i32 m_portionOffsetX = 0;
    i32 m_portionOffsetZ = 0;

    // Rendered area chunks id offset.
    i32 m_renderOffsetX = 0;
    i32 m_renderOffsetZ = 0;

    u32 m_centerChunkX = 0;
    u32 m_centerChunkZ = 0;

}; // struct WorldGfx

} // namespace vox::gfx