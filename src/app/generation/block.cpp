/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:33:58 by etran             #+#    #+#             */
/*   Updated: 2023/12/23 01:20:41 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Block::Block(const MaterialType type) noexcept: _type(type)  {}

/* ========================================================================== */

void	Block::setType(const MaterialType block_type) noexcept {
	_type = block_type;
}

void	Block::setProperies(const MaterialProperty block_type) noexcept {
	_properties = block_type;
}

MaterialType	Block::getType() const noexcept {
	return _type;
}

MaterialProperty	Block::getProperties() const noexcept {
	return _properties;
}

/* ========================================================================== */

uint16_t	Block::computePackedData() const noexcept {
	return (uint32_t)_type << 8 | (uint8_t)_properties;
}

/* GAMEPLAY ================================================================= */

bool	Block::isEmpty() const noexcept {
	return _type == MaterialType::AIR;
}

bool	Block::isSolid() const noexcept {
	return (uint32_t)_properties & (uint32_t)MaterialProperty::SOLID;
}

bool	Block::isFluid() const noexcept {
	return (uint32_t)_properties & (uint32_t)MaterialProperty::FLUID;
}

bool	Block::isDirected() const noexcept {
	return (uint32_t)_properties & (uint32_t)MaterialProperty::DIRECTED;
}

/* GFX ====================================================================== */

} // namespace vox