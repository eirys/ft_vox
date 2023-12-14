/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_properties.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 21:28:34 by etran             #+#    #+#             */
/*   Updated: 2023/12/11 21:57:41 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <cstdint> // uint8_t

# include "culling_consts.h"

# define BLOCK_SIZE	1

namespace vox {

/* ========================================================================== */
/*                                 BLOCK TYPES                                */
/* ========================================================================== */

// Total material types possible: 256
enum class MaterialType: uint8_t {
	AIR		= 0,

	// Solid
	GRASS,
	DIRT,
	STONE,
	SAND,
	SNOW,

	// Directed
	WOOD,

	// Liquid
	WATER,
	LAVA,

	// Transparent
	GLASS,
	LEAVES,
	CLOUD,

	Last = CLOUD
};

/* ========================================================================== */
/*                                 PROPERTIES                                 */
/* ========================================================================== */

enum class MaterialGFXProperty: uint8_t {
	Default				= 0,

	TRANSPARENT			= TRANSPARENT_MASK,
	ALPHACLIPPED		= ALPHACLIPPED_MASK,
	LIGHT				= LIGHT_MASK,

	PropertyShift		= 24
};

enum class MaterialState: uint8_t {
	Default				= 0,

	SOLID				= Default,
	FLUID				= 0x01,
	HALF_FULL			= 0x02,
	HOT					= 0x04,

	PropertyShift		= 16
};

enum class MaterialInteraction: uint8_t {
	Default				= 0,

	GRAVITY_AFFECTED	= 0x01,
	COLLIDABLE			= 0x02,

	PropertyShift		= 8
};

enum class MaterialAspect: uint8_t {
	Default				= 0,

	DIRECTED			= 0x01,
	COLORED				= 0x02,

	PropertyShift		= 0
};

/**
 * @brief Material properties: uint32_t
 * @param GFX			0xFF000000
 * @param State			0x00FF0000
 * @param Interaction	0x0000FF00
 * @param Aspect		0x000000FF
*/
enum class MaterialProperty: uint32_t {
	// GFX
	TRANSPARENT			= MaterialGFXProperty::TRANSPARENT << MaterialGFXProperty::PropertyShift,
	ALPHACLIPPED		= MaterialGFXProperty::ALPHACLIPPED << MaterialGFXProperty::PropertyShift,
	LIGHT_SOURCE		= MaterialGFXProperty::LIGHT << MaterialGFXProperty::PropertyShift,

	MaterialGFXPropertyMask = 0xFF000000,

	// State
	SOLID				= MaterialState::SOLID << MaterialState::PropertyShift,
	FLUID				= MaterialState::FLUID << MaterialState::PropertyShift,

	MaterialStateMask	= 0x00FF0000,

	// Interaction
	GRAVITY_AFFECTED	= MaterialInteraction::GRAVITY_AFFECTED << MaterialInteraction::PropertyShift,
	COLLIDABLE			= MaterialInteraction::COLLIDABLE << MaterialInteraction::PropertyShift,

	MaterialInteractionMask = 0x0000FF00,

	// Aspect
	DIRECTED			= MaterialAspect::DIRECTED << MaterialAspect::PropertyShift,
	COLORED				= MaterialAspect::COLORED << MaterialAspect::PropertyShift,

	MaterialAspectMask	= 0x000000FF
};

} // namespace vox