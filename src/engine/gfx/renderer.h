/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:17:21 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 14:00:10 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core.h"
#include "device.h"
#include "pipeline_decl.h"
#include "sync_decl.h"
#include "fence.h"
#include "gfx_semaphore.h"
#include "swap_chain.h"
#include "descriptor_table.h"

namespace ui {

class Window;

} // namespace ui


namespace vox::gfx {

class Pipeline;

/**
 * @brief Holds all the pipelines and render the scene.
 */
class Renderer final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Renderer();
    ~Renderer();

    Renderer(Renderer&& other) = delete;
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;

    /* ====================================================================== */

    void init(ui::Window& window);
    void destroy();

    void waitIdle() const;
    void render();

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Core                                    m_core;
    Device                                  m_device;
    SwapChain                               m_swapChain;

    std::array<Fence, FENCE_COUNT>          m_fences;
    std::array<GfxSemaphore, FENCE_COUNT>   m_semaphores;

    std::array<Pipeline*, PIPELINE_COUNT>   m_pipelines;
    VkPipelineLayout                        m_pipelineLayout;

    DescriptorTable                         m_descriptorTable;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createPipelines();
    void    _createPipelineLayout();
    void    _createGfxSemaphores();
    void    _createFences();

    void    _destroyGfxSemaphores();
    void    _destroyFences();
    void    _destroyPipelineLayout();
    void    _destroyPipelines();

}; // class Renderer

} // namespace vox::gfx