/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pfd_ubo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:46:39 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 21:11:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "matrix.h"
#include "vector.h"
#include "vox_decl.h"

namespace vox::gfx {

struct PFDUbo final {
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    struct {
        math::Mat4  view;
        math::Mat4  proj;
    }           m_cameraViewProj;

    struct {
        math::Vect2 sunPos;
        u32         skyHue;
        u32         padding[13];
    }           m_gameData;

#if ENABLE_SHADOW_MAPPING
    struct {
        math::Mat4  view;
        math::Mat4  proj;
    }           m_projectorViewProj;
#endif

    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Offset: u32 {
        CameraViewProj      = 0,
        GameData            = CameraViewProj + sizeof(m_cameraViewProj),
#if ENABLE_SHADOW_MAPPING
        ProjectorViewProj   = GameData + sizeof(m_gameData)
#endif
    };

}; // struct PFDUbo

static_assert(sizeof(PFDUbo) % 0x40 == 0, "PFD UBO size is must be a multiple of 64 bytes. Check value of PADDING_NEEDED.");

// For missing padding:
constexpr u32 PADDING_NEEDED = (0x40 - (sizeof(PFDUbo) % 0x40)) / sizeof(u32);

} // namespace vox::gfx
