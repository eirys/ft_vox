#pragma once

#include "bounding_box.h"
#include "vertex.h"

namespace vox::gfx {

class BoundingFrustum;

struct ChunkGfx final {
    BoundingBox m_boundingBox;
    u32         m_instanceCount = 0;
    u32         m_instanceOffset = 0;
    bool        m_updated = false;

    static u32 chunksDrawn;

}; // struct ChunkGfx

} // namespace vox::gfx
