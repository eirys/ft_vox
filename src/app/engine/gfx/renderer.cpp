/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 09:29:35 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 00:47:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "renderer.h"
#include "scene_pipeline.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Renderer::init(ui::Window& window) {
    m_core.init(window);
    m_device.init(m_core);
    _createPipelines();

    LINFO("Renderer initialized.");
}

void Renderer::destroy() {
    _destroyPipelines();
    m_device.destroy();
    m_core.destroy();
    LINFO("Renderer destroyed.");
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void Renderer::_createPipelines() {
    m_pipelines[(u32)PipelineIndex::ScenePipeline] = new ScenePipeline();

    for (Pipeline* pipeline: m_pipelines) pipeline->init(m_device);
}

void Renderer::_destroyPipelines() {
    for (u32 i = 0; i < PIPELINE_COUNT; i++) {
        m_pipelines[i]->destroy(m_device);
        delete m_pipelines[i];
    }
}

} // namespace vox::gfx