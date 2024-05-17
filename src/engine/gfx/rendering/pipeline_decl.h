/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_decl.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:37:23 by etran             #+#    #+#             */
/*   Updated: 2024/03/31 16:03:44 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

#include "types.h"
#include "enum.h"

namespace vox::gfx {

/**
 * @brief List of all pipelines.
*/
enum class PipelineIndex: u32 {
    ScenePipeline = 0,
    SkyboxPipeline,
    StarfieldPipeline,

    First = ScenePipeline,
    Last = StarfieldPipeline
};

constexpr u32 PIPELINE_COUNT = enumSize<PipelineIndex>();

enum class ShaderType: u8 {
    VS = VK_SHADER_STAGE_VERTEX_BIT,
    FS = VK_SHADER_STAGE_FRAGMENT_BIT,
    CS = VK_SHADER_STAGE_COMPUTE_BIT
};

} // namespace vox::gfx
