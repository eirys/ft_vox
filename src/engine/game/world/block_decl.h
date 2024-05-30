/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_decl.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 16:45:00 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 17:01:18 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"

namespace game {

enum class BlockFace: u8 {
    Top = 0,
    Bottom,
    Left,
    Right,
    Front,
    Back
};

// Total material types possible: 7
enum class MaterialType: u8 {
	AIR = 0,

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
	// LAVA,

	// // Transparent
	// GLASS,
	// LEAVES,
	// CLOUD,

	First = AIR,
	Last = WATER
};

static_assert((u8)MaterialType::Last < 8, "MaterialType::Last must be less than 8");

} // namespace game