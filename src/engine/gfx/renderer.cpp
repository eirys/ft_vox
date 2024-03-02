/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 09:29:35 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 12:36:46 by etran            ###   ########.fr       */
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
    _createFences();
    _createGfxSemaphores();

    LINFO("Renderer initialized.");
}

void Renderer::destroy() {
    _destroyGfxSemaphores();
    _destroyFences();
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

void Renderer::_createGfxSemaphores() {
    for (GfxSemaphore& semaphore : m_semaphores) semaphore.init(m_device);
}

void Renderer::_createFences() {
    for (Fence& fence : m_fences) fence.init(m_device);
}

/* ========================================================================== */

void Renderer::_destroyPipelines() {
    for (u32 i = 0; i < PIPELINE_COUNT; i++) {
        m_pipelines[i]->destroy(m_device);
        delete m_pipelines[i];
    }
}

void Renderer::_destroyGfxSemaphores() {
    for (GfxSemaphore& semaphore : m_semaphores) semaphore.destroy(m_device);
}

void Renderer::_destroyFences() {
    for (Fence& fence : m_fences) fence.destroy(m_device);
}

} // namespace vox::gfx