/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_properties.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 21:28:34 by etran             #+#    #+#             */
/*   Updated: 2024/01/11 16:29:06 by etran            ###   ########.fr       */
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
	AIR		= EMPTY_BLOCK,

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

	First = AIR,
	Last = CLOUD
};

/* ========================================================================== */
/*                                 PROPERTIES                                 */
/* ========================================================================== */

enum class MaterialGFXProperty: uint8_t {
	Default				= 0,

	OPAQUE				= Default,
	TRANSPARENT			= TRANSPARENT_MASK,
	ALPHACLIPPED		= ALPHACLIPPED_MASK,
	LIGHT				= LIGHT_MASK,

	PropertyShift		= 24
};

enum class MaterialState: uint8_t {
	Default				= 0,

	VOID				= Default,
	SOLID				= 0x01,
	FLUID				= 0x02,
	HOT					= 0x03,

	PropertyShift		= 16
};

enum class MaterialInteraction: uint8_t {
	Default				= 0,

	INTANGIBLE			= Default,
	GRAVITY_AFFECTED	= 0x01,
	COLLIDABLE			= 0x02,

	PropertyShift		= 8
};

enum class MaterialAspect: uint8_t {
	Default				= 0,

	REGULAR				= Default,
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
	OPAQUE				= (uint32_t)MaterialGFXProperty::OPAQUE << (uint32_t)MaterialGFXProperty::PropertyShift,
	TRANSPARENT			= (uint32_t)MaterialGFXProperty::TRANSPARENT << (uint32_t)MaterialGFXProperty::PropertyShift,
	ALPHACLIPPED		= (uint32_t)MaterialGFXProperty::ALPHACLIPPED << (uint32_t)MaterialGFXProperty::PropertyShift,
	LIGHT_SOURCE		= (uint32_t)MaterialGFXProperty::LIGHT << (uint32_t)MaterialGFXProperty::PropertyShift,

	// State
	VOID				= (uint32_t)MaterialState::VOID << (uint32_t)MaterialState::PropertyShift,
	SOLID				= (uint32_t)MaterialState::SOLID << (uint32_t)MaterialState::PropertyShift,
	FLUID				= (uint32_t)MaterialState::FLUID << (uint32_t)MaterialState::PropertyShift,
	HOT					= (uint32_t)MaterialState::HOT << (uint32_t)MaterialState::PropertyShift,

	// Interaction
	INTANGIBLE			= (uint32_t)MaterialInteraction::INTANGIBLE << (uint32_t)MaterialInteraction::PropertyShift,
	GRAVITY_AFFECTED	= (uint32_t)MaterialInteraction::GRAVITY_AFFECTED << (uint32_t)MaterialInteraction::PropertyShift,
	COLLIDABLE			= (uint32_t)MaterialInteraction::COLLIDABLE << (uint32_t)MaterialInteraction::PropertyShift,

	// Aspect
	REGULAR				= (uint32_t)MaterialAspect::REGULAR << (uint32_t)MaterialAspect::PropertyShift,
	DIRECTED			= (uint32_t)MaterialAspect::DIRECTED << (uint32_t)MaterialAspect::PropertyShift,
	COLORED				= (uint32_t)MaterialAspect::COLORED << (uint32_t)MaterialAspect::PropertyShift,

	GFXPropertyMask		= 0xFF000000,
	StateMask			= 0x00FF0000,
	InteractionMask		= 0x0000FF00,
	AspectMask			= 0x000000FF,

	Default				= OPAQUE | SOLID | INTANGIBLE | REGULAR,

	First = OPAQUE,
	Last = COLORED
};

} // namespace vox