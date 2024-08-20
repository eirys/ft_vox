/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:24:42 by etran             #+#    #+#             */
/*   Updated: 2024/08/20 13:44:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "chunk.h"
#include "world_gfx.h"

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
    static constexpr u32 DIMENSION = SIZE * SIZE;

    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct Settings {

        struct Generation {
            static constexpr u32 SEED = 42; // TODO: remove
            static constexpr u32 NUMBER_OF_CHUNKS = 1024; // Number of chunks
            static constexpr u32 BLOCK_COUNT = 1024 * Chunk::SIZE; //??
        } generation;

        struct Rendering {
            u32 renderDistance = 8; // min-max 1 - 24
        } rendering;

    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void init();
    void ignoreData();

    /* ====================================================================== */

    const math::Vect3&          getOrigin() const noexcept;
    const std::vector<Chunk>&   getChunks() const noexcept;
    std::vector<Chunk>&         getChunks() noexcept;

    Chunk&              getChunk(const u32 x, const u32 z) noexcept;
    const Chunk&        getChunk(const u32 x, const u32 z) const noexcept;

    const std::vector<vox::gfx::VertexInstance>&    getInstances() const noexcept;
    std::vector<u8>                                 getBlockRaw() const;

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

    void _generateInstances();

}; // class World

} // namespace game