/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_decl.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:37:23 by etran             #+#    #+#             */
/*   Updated: 2024/06/16 18:56:24 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"
#include "vox_decl.h"

namespace vox::gfx {

/**
 * @brief List of all pipelines.
*/
enum class PipelineIndex: u32 {
    ScenePipeline,

#if ENABLE_SKYBOX
    SkyboxPipeline,
    StarfieldPipeline,
#endif

    Deferred,

#if ENABLE_SHADOW_MAPPING
    ShadowPipeline,
#endif

#if ENABLE_SSAO
    Ssao,
    SsaoBlur,
#endif

    DebugPipeline,

    Count
};

constexpr u32 PIPELINE_COUNT = (u32)PipelineIndex::Count;

// -----------------------

enum class PipelineLayoutIndex: u32 {
    Scene,
    Deferred,

#if ENABLE_SKYBOX
    Sky,
#endif

#if ENABLE_SHADOW_MAPPING
    Shadows,
#endif

#if ENABLE_SSAO
    Ssao,
    SsaoBlur,
#endif

    Count
};

constexpr u32 PIPELINE_LAYOUT_COUNT = (u32)PipelineLayoutIndex::Count;

// -----------------------

enum class PushConstantIndex: u32 {
    Camera,

    Count
};

constexpr u32 PUSH_CONSTANT_COUNT = (u32)PushConstantIndex::Count;

} // namespace vox::gfx
