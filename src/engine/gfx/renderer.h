/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:17:21 by etran             #+#    #+#             */
/*   Updated: 2024/03/21 01:54:48 by etran            ###   ########.fr       */
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

    Renderer();
    ~Renderer();

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
        Transfer,
        // Compute,

        First = Draw,
        Last = Transfer
    };

    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32    CMD_BUFFER_COUNT = enumSize<CommandBufferIndex>();

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Core                                    m_core;
    Device                                  m_device;
    SwapChain                               m_swapChain;
    CommandPool                             m_commandPool;
    DescriptorPool                          m_descriptorPool;

    std::array<Fence, FENCE_COUNT>                  m_fences;
    std::array<GfxSemaphore, SEMAPHORE_COUNT>       m_semaphores;
    std::array<ICommandBuffer*, CMD_BUFFER_COUNT>   m_commandBuffers;
    std::array<Pipeline*, PIPELINE_COUNT>           m_pipelines;
    std::array<RenderPass*, RENDER_PASS_COUNT>      m_renderPasses;

    DescriptorTable                         m_descriptorTable;
    VkPipelineLayout                        m_pipelineLayout;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createCommandBuffers();
    void    _createRenderPasses();
    void    _createPipelines();
    void    _createPipelineLayout();
    void    _createGfxSemaphores();
    void    _createFences();

    void    _destroyFences();
    void    _destroyGfxSemaphores();
    void    _destroyPipelineLayout();
    void    _destroyPipelines();
    void    _destroyRenderPasses();
    void    _destroyCommandBuffers();

}; // class Renderer

} // namespace vox::gfx