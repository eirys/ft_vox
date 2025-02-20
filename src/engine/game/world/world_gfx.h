#pragma once

#include <vector>

#include "vector.h"
#include "vertex.h"

namespace game {
class World;
class Chunk;
}

namespace vox::gfx {

/**
 * @brief Handler for gfx data of the world (vertices, rendering area, etc).
 */
class WorldGfx final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    WorldGfx() = default;
    ~WorldGfx() = default;

    WorldGfx(WorldGfx&& other) = delete;
    WorldGfx(const WorldGfx& other) = delete;
    WorldGfx& operator=(WorldGfx&& rhs) = delete;
    WorldGfx& operator=(const WorldGfx& rhs) = delete;

    /* ====================================================================== */

    void generateInstances(std::vector<game::Chunk>& chunks);
    void computeRenderedAreaChunks(const std::vector<game::Chunk>& chunks);
    void updateRenderData(const math::ivec2& chunkPos);

    /* ====================================================================== */

    const std::vector<VertexInstance>&      getInstances() const noexcept { return m_instances; }
    const std::vector<const game::Chunk*>&  getRenderedChunks() const noexcept { return m_renderedChunks; }
    const math::ivec2&                      getPortionOffset() const noexcept { return m_portionOffset; }
    const math::ivec2&                      getRenderOffset() const noexcept { return m_renderOffset; }
    bool                                    needsUpdate() const noexcept { return m_needUpdate; }

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::vector<VertexInstance>         m_instances;
    std::vector<const game::Chunk*>     m_renderedChunks;

    // Portion of world generated offset. At first, center of full world.
    // Aka PG
    math::ivec2 m_portionOffset = math::ivec2(0, 0);

    // Rendered area chunks id offset.
    // Aka RA
    math::ivec2 m_renderOffset = math::ivec2(0, 0);

    bool        m_needUpdate = false;


}; // class WorldGfx

} // namespace vox::gfx