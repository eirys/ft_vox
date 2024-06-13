/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_decl.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:55:18 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 10:05:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"
#include "enum.h"
#include "vox_decl.h"

namespace vox::gfx {

enum class RenderPassIndex: u32 {
    Main,

#if ENABLE_SHADOW_MAPPING
    Shadow,
#endif

    Count
};

constexpr u32 RENDER_PASS_COUNT = (u32)RenderPassIndex::Count;

struct RecordInfo {
    virtual ~RecordInfo() = default;

    u32 m_targetIndex = 0;
};

} // namespace vox::gfx
