/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:10:34 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 17:12:53 by etran            ###   ########.fr       */
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

    Block(const MaterialType material);

    Block() = default;
    ~Block() = default;
    Block(Block&& other) = default;
    Block& operator=(Block&& other) = default;

    Block(const Block& other) = delete;
    Block& operator=(const Block& other) = delete;

    /* ====================================================================== */

    bool            isVoid() const noexcept;
    MaterialType    getMaterial() const noexcept;
    void            setMaterial(const MaterialType newMaterial) noexcept;

    u8              getTextureId(const BlockFace face) const noexcept;

    explicit operator MaterialType() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::array<u8, 6>   m_textureId = { 0, 0, 0, 0, 0, 0 };
    MaterialType        m_material = MaterialType::AIR;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void            _setTextureId() noexcept;

}; // class Block

} // namespace game