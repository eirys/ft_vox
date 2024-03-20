/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_ubo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:46:39 by etran             #+#    #+#             */
/*   Updated: 2024/03/20 19:15:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "matrix.h"
#include "vector.h"

namespace vox::gfx {

struct MvpUbo final {

    enum class Offset: u32 {
        ViewProj    = 0,
        GameData    = ViewProj + sizeof(math::Mat4),
    };

    math::Mat4          m_viewProj;
    struct {
        f32             m_time;
        struct {
            math::Vect3     front;
            math::Vect3     right;
            math::Vect3     up;
        }               m_camera;
        u32             m_padding[2];
    }                   m_gameData;

}; // struct MvpUbo

static_assert(sizeof(MvpUbo) % 16 == 0);

} // namespace vox::gfx
