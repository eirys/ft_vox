/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 09:29:35 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 22:36:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "renderer.h"
#include "pipeline_decl.h"
#include "scene_pipeline.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Renderer::init(ui::Window& window) {
    m_core.init(window);
    m_device.init(m_core);
    _createPipelines();
}

void Renderer::destroy() {
    _destroyPipelines();
    m_device.destroy();
    m_core.destroy();
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void Renderer::_createPipelines() {
    m_pipelines.reserve(PIPELINE_COUNT);
    m_pipelines[(u32)PipelineIndex::ScenePipeline] = new ScenePipeline();

    for (Pipeline* pipeline: m_pipelines) pipeline->init(m_device);
}

void Renderer::_destroyPipelines() {
    for (Pipeline* pipeline: m_pipelines) pipeline->destroy(m_device);
    for (Pipeline* pipeline: m_pipelines) delete pipeline;
}

} // namespace vox::gfx