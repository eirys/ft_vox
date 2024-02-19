/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_data.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 12:50:28 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 14:16:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "block_properties.h"

namespace vox {

/* ========================================================================== */
/*                                 BLOCK TYPES                                */
/* ========================================================================== */

// Total material types possible: 256
enum class MaterialType: uint8_t {
	AIR		= EMPTY_BLOCK,

	// Solid
	GRASS,
	DIRT,
	STONE,
	SNOW,

	// Gravity affected
	SAND,

	// Directed
	WOOD,

	// Liquid
	WATER,
	LAVA,

	// Transparent
	GLASS,
	LEAVES,
	CLOUD,

	First = AIR,
	Last = CLOUD
};

/**
 * @brief Contains block data (depending on type).
 * @todo Bound to become abstract and have children.
 */
class BlockData {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	BlockData(const MaterialType block_type = MaterialType::AIR);

	BlockData(BlockData &&src) = default;
	BlockData(const BlockData &src) = default;
	BlockData &operator=(BlockData &&rhs) = default;
	BlockData &operator=(const BlockData &rhs) = default;
	~BlockData() = default;

	/* ========================================================================= */

	void				setType(const MaterialType block_type) noexcept;

	MaterialType		getType() const noexcept;
	MaterialGFXProperty	getGFXProperty() const noexcept;
	MaterialState		getStateProperty() const noexcept;
	MaterialInteraction	getInteractionProperty() const noexcept;
	MaterialAspect		getAspectProperty() const noexcept;

private:
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	MaterialType		_type			= MaterialType::AIR;
	MaterialGFXProperty	_gfx			= MaterialGFXProperty::TRANSPARENT;
	MaterialState		_state			= MaterialState::VOID;
	MaterialInteraction	_interaction	= MaterialInteraction::INTANGIBLE;
	MaterialAspect		_aspect			= MaterialAspect::REGULAR;

}; // class BlockData

} // namespace vox