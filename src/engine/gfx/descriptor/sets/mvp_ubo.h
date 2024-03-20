/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_ubo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:46:39 by etran             #+#    #+#             */
/*   Updated: 2024/03/20 19:36:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "matrix.h"
#include "vector.h"

namespace vox::gfx {

struct MvpUbo final {
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Offset: u32 {
        ViewProj    = 0,
        GameData    = ViewProj + sizeof(math::Mat4),
    };

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    math::Mat4  m_viewProj;

    struct {
        f32 time;
        struct {
            math::Vect3     front;
            math::Vect3     right;
            math::Vect3     up;
        }   camera;
        u32 padding[2];
    }           m_gameData;

}; // struct MvpUbo

static_assert(sizeof(MvpUbo) % 16 == 0);

} // namespace vox::gfx
