/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:17:21 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 12:31:29 by etran            ###   ########.fr       */
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
#include "descriptor_pool.h"
#include "command_pool.h"

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

    void init(ui::Window& window, const GameState& game);
    void destroy();

    void waitIdle() const;
    void render();

private:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class CommandBufferIndex: u32 {
        Draw = 0,
        // Compute,

        First = Draw,
        Last = Draw
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

    DescriptorTable                         m_descriptorTable;
    VkPipelineLayout                        m_pipelineLayout;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createCommandBuffers();
    void    _createPipelines();
    void    _createPipelineLayout();
    void    _createGfxSemaphores();
    void    _createFences();

    void    _destroyFences();
    void    _destroyGfxSemaphores();
    void    _destroyPipelineLayout();
    void    _destroyPipelines();
    void    _destroyCommandBuffers();

}; // class Renderer

} // namespace vox::gfx