/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 16:49:09 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 13:59:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

#include <cassert>

namespace game {

static constexpr
u8 TEXTURE_IDS[(u32)MaterialType::Count][6] = {
    { 5, 5, 5, 5, 5, 5 }, // Air
    { 2, 0, 1, 1, 1, 1 }, // Grass
    { 0, 0, 0, 0, 0, 0 }, // Dirt
    { 3, 3, 3, 3, 3, 3 }, // Stone
    { 5, 5, 5, 5, 5, 5 }, // Snow
    { 4, 4, 4, 4, 4, 4 }, // Sand
    { 5, 5, 5, 5, 5, 5 }, // Wood
    { 5, 5, 5, 5, 5, 5 }, // Water
};

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Block::Block(const MaterialType material, const Biome biome):
    m_material(material),
    m_biome(biome) {}

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
    return TEXTURE_IDS[(u32)m_material][(u8)face];
}

Biome Block::getBiome() const noexcept {
    return m_biome;
}

} // namespace game