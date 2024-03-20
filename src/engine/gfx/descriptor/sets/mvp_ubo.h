/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_ubo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:46:39 by etran             #+#    #+#             */
/*   Updated: 2024/03/20 18:52:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "matrix.h"
#include "vector.h"

namespace vox::gfx {

struct MvpUbo final {

    enum class Offset: u32 {
        ViewProj    = 0,
        Camera      = ViewProj + sizeof(math::Mat4),
    };

    math::Mat4          m_viewProj;
    struct {
        f32             m_time;
        math::Vect3     front;
        math::Vect3     right;
        math::Vect3     up;
        u32             padding[2];
    }                   m_camera;

}; // struct MvpUbo

static_assert(sizeof(MvpUbo) % 16 == 0);

} // namespace vox::gfx
