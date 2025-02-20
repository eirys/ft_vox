/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:29:06 by etran             #+#    #+#             */
/*   Updated: 2024/09/17 15:21:21 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"
#include "game_decl.h"
#include "block.h"
#include "chunk_gfx.h"

#include <vector>

namespace proc {
class VoxMap;
}

namespace game {

enum ChunkNeighbor: u32 {
    Left = 0,
    Right,
    Front,
    Back,

    Count
};

class Chunk final {
public:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr bool SHOULD_STORE_BLOCKS = false;
    static constexpr u32 HEIGHT = CHUNK_HEIGHT;
    static constexpr u32 SIZE = CHUNK_SIZE;
    static constexpr u32 AREA = SIZE * SIZE;
    static constexpr u32 VOLUME = SIZE * SIZE * HEIGHT;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Chunk() = default;
    ~Chunk() = default;
    Chunk(Chunk&& other) = default;
    Chunk& operator=(Chunk&& other) = default;

    Chunk(const Chunk& other) = delete;
    Chunk& operator=(const Chunk& other) = delete;

    /* ====================================================================== */

    void    generateTerrain(
        const proc::VoxMap& terrainNoise,
        const proc::VoxMap& biomeMap,
        const u32 offsetX,
        const u32 offsetZ);


    void   generateInstances(
        std::vector<vox::gfx::VertexInstance>& instances,
        const std::vector<const Chunk*>& neighbors);

    void    clearBlocks();

    // DPCT
    void    generate(
        const proc::VoxMap& terrainNoise,
        const proc::VoxMap& biomeMap,
        const u32 offsetX,
        const u32 offsetZ);

    /* ====================================================================== */

    Block&          operator[](const u32 index) noexcept;
    const Block&    operator[](const u32 index) const noexcept;
    Block&          getBlock(const u32 x, const u32 y, const u32 z) noexcept;
    const Block&    getBlock(const u32 x, const u32 y, const u32 z) const noexcept;

    const std::vector<Block>&       getBlocks() const;
    u16                             getId() const;
    bool                            isVisible(const vox::gfx::BoundingFrustum& frustum) const noexcept { return m_chunkGfx.m_boundingBox.isVisible(frustum); }
    u32                             getInstanceCount() const noexcept { return m_chunkGfx.m_instanceCount; }
    u32                             getInstanceOffset() const noexcept { return m_chunkGfx.m_instanceOffset; }

    /* ====================================================================== */

    void                cache() const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    vox::gfx::ChunkGfx      m_chunkGfx;

    std::vector<Block>      m_blocks;
    math::uvec2             m_position;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Biome   _getBiome(const f32 cellValue, const f32 moistureValue) const noexcept;
    u8      _getHeight(const proc::VoxMap& terrainNoise, const Biome biome, const u32 x, const u32 z) const noexcept;

    MaterialType _getMaterial(const Biome biome, const u8 height) const noexcept;

}; // class Chunk

} // namespace game