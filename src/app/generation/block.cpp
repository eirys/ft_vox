/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 11:33:58 by etran             #+#    #+#             */
/*   Updated: 2023/09/19 11:33:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Block::Block(MaterialType type) noexcept: _type(type)  {}

/* ========================================================================== */

void	Block::setType(MaterialType block_type) noexcept {
	_type = block_type;
}

MaterialType	Block::getType() const noexcept {
	return _type;
}

uint16_t	Block::getPackedData() const noexcept {
	return (uint32_t)_type << 8 | (uint8_t)_properties;
}

/* ========================================================================== */

bool	Block::isEmpty() const noexcept {
	return _type == MaterialType::AIR;
}

} // namespace vox