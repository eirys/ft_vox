/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 20:48:34 by etran             #+#    #+#             */
/*   Updated: 2023/07/17 21:49:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "cube.h"

namespace vox {

/**
 * @brief Materials (up to 256)
*/
enum MaterialType: uint8_t {
	MATERIAL_GRASS,
	MATERIAL_DIRT,
	MATERIAL_STONE,
	MATERIAL_WOOD,
	MATERIAL_LEAVES,
	MATERIAL_SAND,
	MATERIAL_WATER,
	MATERIAL_LAVA,
	MATERIAL_ICE,
	MATERIAL_SNOW
};

/**
 * @brief Holds data about a block: position and material.
*/
class Block {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Block(Cube& cube, MaterialType material);

	Block(const Block& src) = default;
	Block(Block&& src) = default;
	Block&operator=(const Block& rhs) = default;
	Block&operator=(Block&& rhs) = default;
	virtual ~Block() = default;

	Block() = delete;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint8_t			_x;
	uint8_t			_y;
	uint8_t			_z;
	MaterialType	_material;

}; // class Block

class Dirt: public Block {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

}; // class Dirt

} // namespace vox