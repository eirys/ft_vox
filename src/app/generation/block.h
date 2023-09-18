/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:08:29 by etran             #+#    #+#             */
/*   Updated: 2023/09/18 17:06:39 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "cube.h"

# define BLOCK_SIZE	1

namespace vox {

class Block final {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Block(uint8_t x, uint8_t y, uint8_t z, MaterialType type) noexcept;

	Block(Block &&src) = default;
	Block &operator=(Block &&rhs) = default;
	~Block() = default;

	Block() = delete;
	Block(const Block &src) = delete;
	Block &operator=(const Block &rhs) = delete;

	/* ========================================================================= */

	uint32_t		packCoordinates() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint8_t			_x;
	uint8_t			_y;
	uint8_t			_z;
	MaterialType	_type;

}; // class Block

} // namespace vox