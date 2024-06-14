/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_decl.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:37:23 by etran             #+#    #+#             */
/*   Updated: 2024/06/13 18:37:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

#include "types.h"
#include "enum.h"
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

    Offscreen,

#if ENABLE_SHADOW_MAPPING
    ShadowPipeline,
#endif

    DebugPipeline,

    Count
};

enum class PipelineLayoutIndex: u32 {
    Main,

#if ENABLE_SSAO
    Offscreen,
    SSAO,
    SSAOBlur,
#endif

    Count
};

constexpr u32 PIPELINE_COUNT = (u32)PipelineIndex::Count;
constexpr u32 PIPELINE_LAYOUT_COUNT = (u32)PipelineLayoutIndex::Count;

enum class ShaderType: u8 {
    VS = VK_SHADER_STAGE_VERTEX_BIT,
    FS = VK_SHADER_STAGE_FRAGMENT_BIT,
    CS = VK_SHADER_STAGE_COMPUTE_BIT
};

} // namespace vox::gfx
