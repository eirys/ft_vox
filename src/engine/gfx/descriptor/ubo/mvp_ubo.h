/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_ubo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:46:39 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 20:49:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ubo.h"
#include "vector.h"

namespace vox::gfx::ubo {

class MvpUbo final: public Ubo {
public:
    void pack(const math::Vect3& col) {
        color = 0xFF000000 | (u32)(col.x) << 16 | (u32)(col.y) << 8 | (u32)(col.z);
    }

    u32 getColor() const noexcept { return color; }

private:
    // Format : ARGB
    u32 color = 0x110000ff;
}; // class MvpUbo

} // namespace vox::gfx::ubo
