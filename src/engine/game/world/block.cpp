/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 16:49:09 by etran             #+#    #+#             */
/*   Updated: 2024/08/15 16:38:22 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

namespace game {

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

Biome Block::getBiome() const noexcept {
    return m_biome;
}

} // namespace game