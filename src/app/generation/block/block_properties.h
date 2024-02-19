/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_properties.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 21:28:34 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 20:24:59 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <cstdint> // uint8_t

# include "culling_consts.h"

# define BLOCK_SIZE	1

namespace vox {

/* ========================================================================== */
/*                                 BLOCK FACES                                */
/* ========================================================================== */

enum class BlockFace: uint8_t {
	RIGHT = 0,
	LEFT,
	TOP,
	BOTTOM,
	FRONT,
	BACK,

	First = RIGHT,
	Last = BACK
};

/* ========================================================================== */
/*                                 PROPERTIES                                 */
/* ========================================================================== */

enum class MaterialGFXProperty: uint8_t {
	Default				= 0,

	// 4 bits for opacity
	OPAQUE				= Default,
	TRANSPARENT			= TRANSPARENT_MASK,		// Has alpha
	ALPHACLIPPED		= ALPHACLIPPED_MASK,	// Uses alpha clipping

	// 4 bits for light
	LIGHT				= LIGHT_MASK,			// Emits light

	OpacityMask			= 0x0F,
	LightMask			= 0xF0,
	PropertyShift		= 24
};

enum class MaterialState: uint8_t {
	Default				= 0,

	VOID				= Default,
	SOLID				= 0x01,
	FLUID				= 0x02,
	HOT					= 0x04,

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
	DIRECTED			= 0x01,	// Given by object direction
	COLORED				= 0x02,	// Sample from color map
	VARIATED			= 0x04,	// Sample from noise map
	BIOME_DEPENDENT		= 0x08, // Sample from biome color map

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
	VARIATED			= (uint32_t)MaterialAspect::VARIATED << (uint32_t)MaterialAspect::PropertyShift,
	BIOME_DEPENDENT		= (uint32_t)MaterialAspect::BIOME_DEPENDENT << (uint32_t)MaterialAspect::PropertyShift,

	GFXPropertyMask		= 0xFF000000,
	StateMask			= 0x00FF0000,
	InteractionMask		= 0x0000FF00,
	AspectMask			= 0x000000FF,

	Default				= OPAQUE | SOLID | INTANGIBLE | REGULAR,

	First = OPAQUE,
	Last = COLORED
};

} // namespace vox