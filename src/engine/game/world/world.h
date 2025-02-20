/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:24:42 by etran             #+#    #+#             */
/*   Updated: 2024/09/17 15:22:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "chunk.h"
#include "world_gfx.h"
#include "debug.h"

namespace game {

class Camera;

enum class NearbyChunkIndex: u32 {
    TopLeft,
    TopCenter,
    TopRight,

    MiddleLeft,
    Center,
    MiddleRight,

    BottomLeft,
    BottomCenter,
    BottomRight,

    Count
};

/**
 * @brief The World class represents the game world.
 * Gives information on chunks and their data.
 */
class World final {
public:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 SIZE = 8; // From center chunk to side chunk count
    static constexpr u32 SIDE = 1 + SIZE * 2; // Number of chunks on one side
    static constexpr u32 DIMENSION = SIDE * SIDE; // Number of chunks in portion generated

    static constexpr u32 MAX_CHUNK_COUNT = 1024; // Number of chunks max for world
    static constexpr u32 MAX_BLOCK_COUNT = 1024 * Chunk::SIZE; // Number of blocks total in world (side count)
    static constexpr u32 CENTER_CHUNK = MAX_CHUNK_COUNT / 2; // Center chunk index

    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct Settings {
        struct Generation {
            static constexpr u32 SEED = 42; // TODO: remove
        } generation;

        struct Rendering {
            u32 renderDistance = 8; // min-max 0 - 23
            u32 renderAreaSide = 1 + renderDistance * 2;
            u32 renderArea = renderAreaSide * renderAreaSide;
        } rendering;
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void init();
    void ignoreData();
    void update(const game::Camera& camera);
    void setRenderDistance(u32 newDistance) noexcept;

    /* ====================================================================== */

    const math::vec3&           getOrigin() const noexcept;
    const std::vector<Chunk>&   getChunks() const noexcept;
    std::vector<Chunk>&         getChunks() noexcept;
    Chunk&                      getChunk(const u32 x, const u32 z) noexcept;
    const Chunk&                getChunk(const u32 x, const u32 z) const noexcept;

    static const Settings&      getSettings() noexcept { return ms_settings; }

    /* GFX ================================================================== */

    const std::vector<vox::gfx::VertexInstance>&    getInstances() const noexcept { return m_worldGfx.getInstances(); }
    const std::vector<const Chunk*>&                getRenderedChunks() const noexcept {return m_worldGfx.getRenderedChunks(); }
    const math::ivec2&                              getPortionOffset() const noexcept { return m_worldGfx.getPortionOffset(); }
    const math::ivec2&                              getRenderOffset() const noexcept { return m_worldGfx.getRenderOffset(); }
    bool                                            needsGfxUpdate() const noexcept { return m_worldGfx.needsUpdate(); }

    std::vector<u8> retrieveBlocksRaw() const;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static Settings         ms_settings;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    vox::gfx::WorldGfx      m_worldGfx;
    std::vector<Chunk>      m_chunks;

    math::vec3             m_origin = { 0.0f, 0.0f, 0.0f };

}; // class World

} // namespace game