/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_ubo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:46:39 by etran             #+#    #+#             */
/*   Updated: 2024/03/19 01:53:40 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "matrix.h"

namespace vox::gfx {

struct MvpUbo final {

    enum class Offset: u32 {
        ViewProj    = 0,
        Time        = sizeof(math::Mat4),
    };

    math::Mat4  viewProj;
    f32         time;
    u32         padding[3];

}; // struct MvpUbo

static_assert(sizeof(MvpUbo) % 16 == 0);

} // namespace vox::gfx
