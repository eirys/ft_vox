#pragma once

#include <vector>

#include "vertex.h"

namespace vox::gfx {

struct WorldGfx final {

    std::vector<VertexInstance> m_instances;

}; // struct WorldGfx

} // namespace vox::gfx