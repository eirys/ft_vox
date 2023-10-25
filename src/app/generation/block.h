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

// Std
# include <cstdint>

# define BLOCK_SIZE	1

namespace vox {

// Total material types possible: 256
enum class MaterialType: uint8_t {
	MATERIAL_AIR		= 0,

	MATERIAL_GRASS		= 1,
	MATERIAL_DIRT		= 2,
	MATERIAL_STONE		= 3,
	MATERIAL_WOOD		= 4,
	MATERIAL_LEAVES		= 5,
	MATERIAL_SAND		= 6,
	MATERIAL_WATER		= 7,
	MATERIAL_LAVA		= 8,
	MATERIAL_ICE		= 9,
	MATERIAL_SNOW		= 10,

	Last				= 255
};

class Block {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Block(MaterialType type) noexcept;

	Block() = default;
	Block(Block &&src) = default;
	Block &operator=(Block &&rhs) = default;
	~Block() = default;

	Block(const Block &src) = delete;
	Block &operator=(const Block &rhs) = delete;

	/* ========================================================================= */

	void			setType(MaterialType block_type) noexcept;
	MaterialType	getType() const noexcept;

	/* ========================================================================= */

	bool			isEmpty() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	MaterialType	_type = MaterialType::MATERIAL_AIR;

}; // class Block

} // namespace vox