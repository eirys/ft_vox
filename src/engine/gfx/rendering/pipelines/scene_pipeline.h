/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 23:49:38 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 00:31:59 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace vox::gfx {

class ScenePipeline final: public Pipeline {
public:

    void    init(const Device& device) {
        (void)device;
    }
    void    destroy(const Device& device) { (void)device; }

    void    record(
        const VkPipelineLayout layout,
        const ICommandBuffer* cmdBuffer,
        const IPipelineRenderInfo* drawInfo) { (void)layout, (void)cmdBuffer, (void)drawInfo; }

private:

}; // class ScenePipeline

} // namespace vox::gfx
