/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_ubo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:46:39 by etran             #+#    #+#             */
/*   Updated: 2024/03/22 23:05:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "matrix.h"
#include "vector.h"

namespace vox::gfx {

struct MvpUbo final {
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    struct {
        math::Mat4  view;
        math::Mat4  proj;
    }           m_viewProj;

    struct {
        f32 time;
        u32 padding[3];
    }           m_gameData;

    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Offset: u32 {
        ViewProj    = 0,
        GameData    = ViewProj + sizeof(m_viewProj),
    };

}; // struct MvpUbo

static_assert(sizeof(MvpUbo) % 16 == 0);

} // namespace vox::gfx