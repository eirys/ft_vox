/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:17:21 by etran             #+#    #+#             */
/*   Updated: 2024/06/13 19:51:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core.h"
#include "device.h"

#include "pipeline_decl.h"
#include "sync_decl.h"
#include "render_decl.h"

#include "fence.h"
#include "gfx_semaphore.h"
#include "swap_chain.h"
#include "descriptor_table.h"
#include "descriptor_pool.h"
#include "pipeline_layout.h"
#include "command_pool.h"

namespace ui {
class Window;
}

namespace game {
class GameState;
}

namespace vox::gfx {

class Pipeline;
class RenderPass;

/**
 * @brief Holds all the pipelines and render the scene.
 */
class Renderer final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Renderer() = default;
    ~Renderer() = default;

    Renderer(Renderer&& other) = delete;
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;

    /* ====================================================================== */

    void init(ui::Window& window, const game::GameState& game);
    void destroy();

    void waitIdle() const;
    void render(const game::GameState& game);

private:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class CommandBufferIndex: u32 {
        Draw = 0,
        Transfer = Draw,
        Offscreen,
        // Compute,

        Count
    };

    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32    CMD_BUFFER_COUNT = (u32)CommandBufferIndex::Count;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Core                                m_core;
    Device                              m_device;
    SwapChain                           m_swapChain;
    CommandPool                         m_commandPool;
    DescriptorPool                      m_descriptorPool;
    DescriptorTable                     m_descriptorTable;

    std::vector<Fence>                  m_fences;
    std::vector<GfxSemaphore>           m_semaphores;
    std::vector<PipelineLayout>         m_pipelineLayouts;
    std::vector<ICommandBuffer*>        m_commandBuffers;
    std::vector<Pipeline*>              m_pipelines;
    std::vector<RenderPass*>            m_renderPasses;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createCommandBuffers();
    void    _createRenderPasses();
    void    _createPipelines();
    void    _createPipelineLayouts();
    void    _createGfxSemaphores();
    void    _createFences();

    void    _destroyFences();
    void    _destroyGfxSemaphores();
    void    _destroyPipelineLayouts();
    void    _destroyPipelines();
    void    _destroyRenderPasses();
    void    _destroyCommandBuffers();

}; // class Renderer

} // namespace vox::gfx