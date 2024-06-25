/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_decl.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:46:17 by etran             #+#    #+#             */
/*   Updated: 2024/06/25 14:49:21 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vox_decl.h"
#include "types.h"

namespace vox::gfx {

enum class TextureIndex: u32 {
    GameTexture,
    PerlinNoise,

    GBufferPosition,
    GBufferNormal,
    GBufferAlbedo,
    GBufferDepth,

#if ENABLE_CUBEMAP
    SkyCubemap,
#endif

#if ENABLE_SSAO
    GBufferNormalView,
    GBufferPositionView,

    GBufferSSAO,
    GBufferSSAOBlur,
#endif

#if ENABLE_SHADOW_MAPPING
    ShadowMap,
#endif

    Count
};

constexpr u32 TEXTURE_COUNT = (u32)TextureIndex::Count;

} // namespace vox::gfx