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

/* ========================================================================== */

Block::operator bool() const noexcept {
	return _type != MaterialType::MATERIAL_AIR;
}

bool	Block::operator!() const noexcept {
	return _type == MaterialType::MATERIAL_AIR;
}

} // namespace vox