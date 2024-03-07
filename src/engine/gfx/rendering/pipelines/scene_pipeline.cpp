/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 09:48:27 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 10:17:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_pipeline.h"
#include "scene_render_pass.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void ScenePipeline::init(const Device& device) {
    m_renderPass = new SceneRenderPass();
    m_renderPass->init(device);
}

void ScenePipeline::destroy(const Device& device) {
    m_renderPass->destroy(device);
    delete m_renderPass;
}

void ScenePipeline::record(
    const VkPipelineLayout layout,
    const ICommandBuffer* cmdBuffer,
    const IPipelineRenderInfo* drawInfo
) {

}

/* ====================================================================== */

RenderPass* ScenePipeline::getRenderPass() const noexcept {
    return nullptr;
}

} // namespace vox::gfx
