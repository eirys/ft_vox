/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_decl.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 16:45:00 by etran             #+#    #+#             */
/*   Updated: 2024/06/10 19:13:17 by etran            ###   ########.fr       */
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

enum class Biome: u8 {
    Plains,
    Desert,
    RockMountains,
    SnowMountains,
    Oceans,

    Count

    // Forest,
    // Jungle,
    // Snow,
    // Swamp,
    // Tundra,
    // Volcano,
    // Wasteland,
    // Woodland,
};

// Total material types possible: 7
enum class MaterialType: u8 {
    Air,

    // Solid
    Grass,
    Dirt,
    Stone,
    Snow,

    // Gravity Affected
    Sand,

    // Directed
    Wood,

    // Liquid
    Water,
    // Lava,

    // // Transparent
    // Glass,
    // Leaves,
    // Cloud,

    Count
};

static_assert((u8)MaterialType::Count <= 8, "MaterialType::Count must not exceed 8");

} // namespace game