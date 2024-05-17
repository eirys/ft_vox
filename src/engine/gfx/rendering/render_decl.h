/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_decl.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:55:18 by etran             #+#    #+#             */
/*   Updated: 2024/03/21 01:43:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"
#include "enum.h"

namespace vox::gfx {

enum class RenderPassIndex: u32 {
    Main = 0,

    First = Main,
    Last = Main
};

constexpr u32 RENDER_PASS_COUNT = enumSize<RenderPassIndex>();

struct RecordInfo {
    virtual ~RecordInfo() = default;

    u32 m_targetIndex = 0;
};

} // namespace vox::gfx
