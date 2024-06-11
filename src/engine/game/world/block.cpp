/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 16:49:09 by etran             #+#    #+#             */
/*   Updated: 2024/06/11 15:47:04 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

#include <cassert>

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Block::Block(const MaterialType material, const Biome biome):
    m_material(material),
    m_biome(biome) {
    _setTextureId();
}

/* ========================================================================== */

Block::operator MaterialType() const noexcept {
    return m_material;
}

bool Block::isVoid() const noexcept {
    return m_material == MaterialType::Air;
}

/* ========================================================================== */

void Block::setMaterial(const MaterialType newMaterial) noexcept {
    m_material = newMaterial;
}

/* ========================================================================== */

MaterialType Block::getMaterial() const noexcept {
    return m_material;
}

u8  Block::getTextureId(const BlockFace face) const noexcept {
    return m_textureId[(u8)face];
}

Biome Block::getBiome() const noexcept {
    return m_biome;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void Block::_setTextureId() noexcept {
    switch (m_material) {
        case MaterialType::Sand:
            m_textureId = { 4, 4, 4, 4, 4, 4 };
            break;

        case MaterialType::Stone:
            m_textureId = { 3, 3, 3, 3, 3, 3 };
            break;

        case MaterialType::Dirt:
            m_textureId = { 0, 0, 0, 0, 0, 0 };
            break;

        case MaterialType::Grass:
            m_textureId = { 2, 0, 1, 1, 1, 1 };
            break;

        case MaterialType::Air:
        default:
            assert(false && "Invalid material type");
    }

}

} // namespace game