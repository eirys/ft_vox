/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:33:58 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 20:26:31 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Block::Block(const MaterialType type) noexcept: _data(type)  {}

/* ========================================================================== */

void	Block::setType(const MaterialType block_type) noexcept {
	_data.setType(block_type);
}

MaterialType	Block::getType() const noexcept {
	return _data.getType();
}

/* ========================================================================== */

/**
 * @brief Packs type and properties into a single uint16_t.
 * @note Format is [type][properties].
 * @note Extract type with (packed_data >> 8) & 0xFF.
 * @note Extract properties with packed_data & 0xFF.
*/
uint16_t	Block::computePackedData() const noexcept {
	const uint16_t packed_type = (uint16_t)_data.getType() << 8;
	const uint16_t packed_property = (uint16_t)_data.getGFXProperty();
	return packed_type | packed_property;
}

Block	Block::computeFromPackedData(const uint16_t packed_data) {
	const uint8_t type = (packed_data >> 8) & 0xFF;

	Block block((MaterialType)type);
	return block;
}

/* GAMEPLAY ================================================================= */

bool	Block::isEmpty() const noexcept {
	return _data.getType() == MaterialType::AIR;
}

bool	Block::isSolid() const noexcept {
	return (uint8_t)_data.getStateProperty() & (uint8_t)MaterialState::SOLID;
}

bool	Block::isFluid() const noexcept {
	return (uint8_t)_data.getStateProperty() & (uint8_t)MaterialProperty::FLUID;
}

bool	Block::isDirected() const noexcept {
	return (uint8_t)_data.getAspectProperty() & (uint8_t)MaterialProperty::DIRECTED;
}

/* GFX ====================================================================== */

bool	Block::isTransparent() const noexcept {
	return (uint8_t)_data.getGFXProperty() & (uint8_t)MaterialGFXProperty::TRANSPARENT;
}

bool	Block::isSemiTransparent() const noexcept {
	return (uint8_t)_data.getGFXProperty() & (uint8_t)MaterialGFXProperty::ALPHACLIPPED;
}

bool	Block::isOpaque() const noexcept {
	return (uint8_t)_data.getGFXProperty() & (uint8_t)MaterialGFXProperty::OPAQUE;
}

bool	Block::isLightSource() const noexcept {
	return (uint8_t)_data.getGFXProperty() & (uint8_t)MaterialGFXProperty::LIGHT;
}

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

#ifdef __DEBUG
std::ostream&		operator<<(std::ostream& os, const Block& block) {
	os << '[' << (uint32_t)block.getType() << ']';
	return os;
}
#endif

} // namespace vox