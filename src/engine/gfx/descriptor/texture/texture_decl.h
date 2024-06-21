/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_decl.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:46:17 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 01:29:19 by etran            ###   ########.fr       */
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
    GBufferNormalView,
    GBufferAlbedo,

#if ENABLE_CUBEMAP
    SkyCubemap,
#endif

#if ENABLE_SSAO
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