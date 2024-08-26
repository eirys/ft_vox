/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:24:42 by etran             #+#    #+#             */
/*   Updated: 2024/08/26 12:32:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "chunk.h"
#include "world_gfx.h"
#include"debug.h"

namespace ui {
class Controller;
} // namespace ui

namespace game {

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

    static constexpr u32 SIZE = 8;
    static constexpr u32 SIDE = 1 + SIZE * 2;
    static constexpr u32 DIMENSION = 1 + 4 * SIZE * (SIZE + 1);

    static constexpr u32 MAX_CHUNK_COUNT = 1024; // Number of chunks max for world
    static constexpr u32 MAX_BLOCK_COUNT = 1024 * Chunk::SIZE; // Number of blocks total in world (side count)

    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct Settings {

        struct Generation {
            static constexpr u32 SEED = 42; // TODO: remove
        } generation;

        struct Rendering {
            u32 renderDistance = 8; // min-max 1 - 23

            u32 getRenderedChunksCount() const {
                return 1 + 4 * renderDistance * (renderDistance + 1);
            }

            u32 getRenderAreaSide() const {
                return 1 + renderDistance * 2;
            }

        } rendering;

    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void init();
    void ignoreData();

    /* ====================================================================== */

    void setRenderDistance(u32 newDistance) noexcept;

    /* ====================================================================== */

    const math::Vect3&          getOrigin() const noexcept;
    const std::vector<Chunk>&   getChunks() const noexcept;
    std::vector<Chunk>&         getChunks() noexcept;
    Chunk&          getChunk(const u32 x, const u32 z) noexcept;
    const Chunk&    getChunk(const u32 x, const u32 z) const noexcept;

    // GFX
    const std::vector<vox::gfx::VertexInstance>&    getInstances() const noexcept;
    std::vector<u8>                                 getBlockRaw() const;
    u32                                             getPortionOffsetX() const noexcept { return m_worldGfx.m_portionOffsetX; }
    u32                                             getPortionOffsetZ() const noexcept { return m_worldGfx.m_portionOffsetZ; }
    u32                                             getRenderOffsetX() const noexcept { return m_worldGfx.m_renderOffsetX; }
    u32                                             getRenderOffsetZ() const noexcept { return m_worldGfx.m_renderOffsetZ; }

    static const Settings& getSettings() noexcept;

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

    // std::vector<Chunk>      m_activeChunks;
    // std::vector<Chunk*>     m_localChunks;

    math::Vect3             m_origin = { 0.0f, 0.0f, 0.0f };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void _updateGfx();
    void _generateInstances();

}; // class World

} // namespace game