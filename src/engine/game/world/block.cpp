/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 16:49:09 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 17:15:44 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Block::Block(const MaterialType material):
    m_material(material) {
    _setTextureId();
}

/* ========================================================================== */

Block::operator MaterialType() const noexcept {
    return m_material;
}

/* ========================================================================== */

bool Block::isVoid() const noexcept {
    return m_material == MaterialType::AIR;
}

void Block::setMaterial(const MaterialType newMaterial) noexcept {
    m_material = newMaterial;
}

MaterialType Block::getMaterial() const noexcept {
    return m_material;
}

u8  Block::getTextureId(const BlockFace face) const noexcept {
    return m_textureId[(u8)face];
}

void Block::_setTextureId() noexcept {
    switch (m_material) {
        case MaterialType::DIRT:    m_textureId = { 0, 0, 0, 0, 0, 0 }; break;
        case MaterialType::GRASS:   m_textureId = { 2, 0, 1, 1, 1, 1 }; break;
        default:
            break;
    }

}

} // namespace game