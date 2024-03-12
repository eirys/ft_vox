/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_ubo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:46:39 by etran             #+#    #+#             */
/*   Updated: 2024/03/12 10:34:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ubo.h"
#include "vector.h"

namespace vox::gfx::ubo {

class MvpUbo final: public Ubo {
public:
    void pack(const math::Vect3& col, const u8 index) {
        packedData = (u32)index << 24 | (u32)(col.x) << 16 | (u32)(col.y) << 8 | (u32)(col.z);
    }

    u32 getColor() const noexcept { return packedData & 0xFFffFF; }
    u32 getIndex() const noexcept { return packedData >> 24; }

private:
    // Format : INDEX | RGB
    u32 packedData = 0x010000ff;
}; // class MvpUbo

} // namespace vox::gfx::ubo
