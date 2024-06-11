/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:29:06 by etran             #+#    #+#             */
/*   Updated: 2024/06/11 15:42:28 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"
#include "game_decl.h"
#include "block.h"
#include "bounding_box.h"

#include <vector>

namespace proc {
class PerlinNoise;
class VoxMap;
using VoronoiDiagram = VoxMap;
}

namespace game {

class Chunk final {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using BlockArray = std::vector<Block>;

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

    void    generate(
        const proc::PerlinNoise& terrainNoise,
        const proc::VoronoiDiagram& biomeMap,
        const u32 offsetX,
        const u32 offsetY,
        const u32 offsetZ) noexcept;

    /* ====================================================================== */

    Block&          operator[](const u32 index) noexcept;
    const Block&    operator[](const u32 index) const noexcept;
    Block&          getBlock(const u32 x, const u32 y, const u32 z) noexcept;
    const Block&    getBlock(const u32 x, const u32 y, const u32 z) const noexcept;

    const BlockArray&   getBlocks() const;
    u16                 getId() const;

    /* ====================================================================== */

    const vox::gfx::BoundingBox&  getBoundingBox() const noexcept;

    /* ====================================================================== */

    void                cache() const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    BlockArray              m_blocks;
    vox::gfx::BoundingBox   m_boundingBox;
    struct {
        u32 m_x = 0;
        u32 m_y = 0;
        u32 m_z = 0;
    }                       m_position;
    bool                    m_updated = false;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Biome   _getBiome(const f32 cellValue, const f32 moistureValue) const noexcept;
    u8      _generateHeight(const proc::PerlinNoise& terrainNoise, const Biome biome, const u32 x, const u32 z) const noexcept;

    MaterialType _getMaterial(const Biome biome, const u8 height) const noexcept;

}; // class Chunk

} // namespace game