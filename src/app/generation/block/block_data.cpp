/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_data.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 13:01:20 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 14:01:43 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block_data.h"
#include "utils.h"

#include <cassert> // assert

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

BlockData::BlockData(const MaterialType block_type) {
	setType(block_type);
}

/* ========================================================================== */

void	BlockData::setType(const MaterialType block_type) noexcept {
	_type = block_type;

	switch (block_type) {
		case MaterialType::AIR:
			_gfx = MaterialGFXProperty::TRANSPARENT;
			_state = MaterialState::VOID;
			_interaction = MaterialInteraction::INTANGIBLE;
			_aspect = MaterialAspect::REGULAR;
			break;

		// Solid
		case MaterialType::DIRT:
			_gfx = MaterialGFXProperty::OPAQUE;
			_state = MaterialState::SOLID;
			_interaction = MaterialInteraction::COLLIDABLE;
			_aspect = MaterialAspect::VARIATED;
			break;

		case MaterialType::GRASS:
			_gfx = MaterialGFXProperty::OPAQUE;
			_state = MaterialState::SOLID;
			_interaction = MaterialInteraction::COLLIDABLE;
			_aspect = MaterialAspect::BIOME_DEPENDENT;
			break;


		case MaterialType::STONE:
			_gfx = MaterialGFXProperty::OPAQUE;
			_state = MaterialState::SOLID;
			_interaction = MaterialInteraction::COLLIDABLE;
			_aspect = MaterialAspect::REGULAR;
			break;

		case MaterialType::SNOW:
			_gfx = MaterialGFXProperty::OPAQUE;
			_state = MaterialState::SOLID;
			_interaction = MaterialInteraction::COLLIDABLE;
			_aspect = MaterialAspect::REGULAR;
			break;

		// Gravity affected
		case MaterialType::SAND:
			_gfx = MaterialGFXProperty::OPAQUE;
			_state = MaterialState::SOLID;
			_interaction = scop::utils::enumToFlag<MaterialInteraction>({MaterialInteraction::COLLIDABLE, MaterialInteraction::GRAVITY_AFFECTED});
			_aspect = MaterialAspect::VARIATED;
			break;

		// Directed
		case MaterialType::WOOD:
			_gfx = MaterialGFXProperty::OPAQUE;
			_state = MaterialState::SOLID;
			_interaction = MaterialInteraction::COLLIDABLE;
			_aspect = MaterialAspect::DIRECTED;
			break;

		// Liquid
		case MaterialType::WATER:
			_gfx = MaterialGFXProperty::TRANSPARENT;
			_state = MaterialState::FLUID;
			_interaction = scop::utils::enumToFlag<MaterialInteraction>({MaterialInteraction::COLLIDABLE, MaterialInteraction::GRAVITY_AFFECTED});
			_aspect = scop::utils::enumToFlag<MaterialAspect>({MaterialAspect::DIRECTED, MaterialAspect::BIOME_DEPENDENT});
			break;

		case MaterialType::LAVA:
			_gfx = MaterialGFXProperty::OPAQUE;
			_state = MaterialState::FLUID;
			_interaction = scop::utils::enumToFlag<MaterialInteraction>({MaterialInteraction::COLLIDABLE, MaterialInteraction::GRAVITY_AFFECTED});
			_aspect = MaterialAspect::DIRECTED;
			break;

		default:
			assert(false && "Invalid block type");
	}
}

/* GETTERS ================================================================== */

MaterialType	BlockData::getType() const noexcept {
	return _type;
}

MaterialGFXProperty	BlockData::getGFXProperty() const noexcept {
	return _gfx;
}

MaterialState		BlockData::getStateProperty() const noexcept {
	return _state;
}

MaterialInteraction	BlockData::getInteractionProperty() const noexcept {
	return _interaction;
}

MaterialAspect		BlockData::getAspectProperty() const noexcept {
	return _aspect;
}

} // namespace vox