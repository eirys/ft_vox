/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:10:34 by etran             #+#    #+#             */
/*   Updated: 2024/06/10 17:17:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "block_decl.h"

#include <array>

namespace game {

class Block final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Block(const MaterialType material, const Biome biome = Biome::Plains);

    Block() = default;
    ~Block() = default;
    Block(Block&& other) = default;
    Block& operator=(Block&& other) = default;

    Block(const Block& other) = delete;
    Block& operator=(const Block& other) = delete;

    /* ====================================================================== */

    void            setMaterial(const MaterialType newMaterial) noexcept;

    u8              getTextureId(const BlockFace face) const noexcept;
    MaterialType    getMaterial() const noexcept;
    Biome           getBiome() const noexcept;

    bool            isVoid() const noexcept;
    explicit operator MaterialType() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::array<u8, 6>   m_textureId = { 0, 0, 0, 0, 0, 0 };

    MaterialType        m_material = MaterialType::Air;
    Biome               m_biome = Biome::Plains;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void            _setTextureId() noexcept;

}; // class Block

} // namespace game