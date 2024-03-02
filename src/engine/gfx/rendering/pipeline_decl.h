/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_decl.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:37:23 by etran             #+#    #+#             */
/*   Updated: 2024/02/29 23:44:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"
#include "enum.h"

namespace vox::gfx {

/**
 * @brief List of all pipelines.
*/
enum class PipelineIndex: u32 {
    ScenePipeline = 0,

    First = ScenePipeline,
    Last = ScenePipeline
};

constexpr u32 PIPELINE_COUNT = enumSize<PipelineIndex>();

} // namespace vox::gfx
