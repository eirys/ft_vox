#include "world_gfx.h"
#include "world.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void WorldGfx::generateInstances(std::vector<game::Chunk>& chunks) {
    auto getChunk = [&chunks](u32 x, u32 z) -> game::Chunk& {
        return chunks[(z * game::World::SIDE) + x];
    };

    using ChunkNeighbor = game::ChunkNeighbor;
    std::vector<const game::Chunk*>  neighbors = {nullptr, nullptr, nullptr, nullptr};

    for (u32 z = 0; z < game::World::SIDE; ++z) {
        for (u32 x = 0; x < game::World::SIDE; ++x) {
            neighbors = {nullptr, nullptr, nullptr, nullptr};

            auto& chunk = getChunk(x, z);

            if (x > 0)
                neighbors[ChunkNeighbor::Left] = &getChunk(x - 1, z);
            if (x < game::World::SIDE - 1)
                neighbors[ChunkNeighbor::Right] = &getChunk(x + 1, z);
            if (z > 0)
                neighbors[ChunkNeighbor::Back] = &getChunk(x, z - 1);
            if (z < game::World::SIDE - 1)
                neighbors[ChunkNeighbor::Front] = &getChunk(x, z + 1);

            chunk.generateInstances(m_instances, neighbors);
        }
    }
}

void WorldGfx::computeRenderedAreaChunks(const std::vector<game::Chunk>& chunks) {
    const auto& settings = game::World::getSettings();

    m_renderedChunks.reserve(settings.rendering.renderArea);

    const u32   sizeDiff = game::World::SIZE - settings.rendering.renderDistance;
    u32         index = sizeDiff * game::World::SIDE;

    if (sizeDiff != 0) index += game::World::SIDE;

    for (u32 i = 0; i < settings.rendering.renderAreaSide; ++i) {
        for (u32 j = 0; j < settings.rendering.renderAreaSide; ++j) {
            m_renderedChunks.emplace_back(&chunks[index]);
            ++index;
        }
        index += sizeDiff * 2;
    }

    LINFO("Rendered area: " << m_renderedChunks.size() << " chunks");
}

void WorldGfx::updateRenderData(const math::ivec2& chunkPos) {
    // IF on the edge of the world, don't update
    if (chunkPos.x != 0) {

        //PG
        if (std::abs(chunkPos.x - m_portionOffset.x) >= game::World::SIZE) {
            // TODO: trigger PG update
            LINFO("Render more world X wise.");
            m_portionOffset = chunkPos;
            m_needUpdate = true;

            return;
        }

        //RA
        if (chunkPos.x != m_renderOffset.x) {
            // TODO: trigger RA update
            // LINFO("Updating render X wise.");
            m_renderOffset.x = chunkPos.x;
            // m_needUpdate = true;

            // return;
        }

    }

    if (chunkPos.y != 0) {

        // PG
        if (std::abs(chunkPos.y - m_portionOffset.y) >= game::World::SIZE) {
            // TODO: trigger PG update
            LINFO("Render more world z wise.");
            m_portionOffset = chunkPos;
            m_needUpdate = true;

            return;
        }

        // RA
        if (m_renderOffset.y != chunkPos.y) {
            // TODO: trigger RA update
            // LINFO("Updating render Z wise.");
            m_renderOffset.y = chunkPos.y;
            // m_needUpdate = true;

            // return;
        }
    }

    m_needUpdate = false;
}

} // namespace vox::gfx