/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:33:58 by etran             #+#    #+#             */
/*   Updated: 2024/01/08 18:52:47 by etran            ###   ########.fr       */
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

void	Block::setGFXProperty(const MaterialGFXProperty property) noexcept {
	_gfx = property;
}

void	Block::setStateProperty(const MaterialState property) noexcept {
	_state = property;
}

void	Block::setInteractionProperty(const MaterialInteraction property) noexcept {
	_interaction = property;
}

void	Block::setAspectProperty(const MaterialAspect property) noexcept {
	_aspect = property;
}


MaterialType	Block::getType() const noexcept {
	return _type;
}

MaterialGFXProperty	Block::getGFXProperty() const noexcept {
	return _gfx;
}

MaterialState		Block::getStateProperty() const noexcept {
	return _state;
}

MaterialInteraction	Block::getInteractionProperty() const noexcept {
	return _interaction;
}

MaterialAspect		Block::getAspectProperty() const noexcept {
	return _aspect;
}


/* ========================================================================== */

/**
 * @brief Packs type and properties into a single uint16_t.
 * @note Format is [type][properties].
 * @note Extract type with (packed_data >> 8) & 0xFF.
 * @note Extract properties with packed_data & 0xFF.
*/
uint16_t	Block::computePackedData() const noexcept {
	const uint16_t packed_data = (uint32_t)_type << 8 | (uint8_t)_gfx;
	return packed_data;
}

/* GAMEPLAY ================================================================= */

bool	Block::isEmpty() const noexcept {
	return _type == MaterialType::AIR;
}

bool	Block::isSolid() const noexcept {
	return (uint8_t)_state & (uint8_t)MaterialState::SOLID;
}

bool	Block::isFluid() const noexcept {
	return (uint8_t)_state & (uint8_t)MaterialProperty::FLUID;
}

bool	Block::isDirected() const noexcept {
	return (uint8_t)_aspect & (uint8_t)MaterialProperty::DIRECTED;
}

/* GFX ====================================================================== */

} // namespace vox