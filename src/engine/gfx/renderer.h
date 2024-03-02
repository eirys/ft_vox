/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:17:21 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 12:36:55 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core.h"
#include "device.h"
#include "pipeline_decl.h"
#include "sync_decl.h"
#include "fence.h"
#include "gfx_semaphore.h"

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

    Renderer() = default;
    ~Renderer() = default;

    Renderer(Renderer&& other) = delete;
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;

    /* ====================================================================== */

    void init(ui::Window& window);
    void destroy();

    void render();

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Core                                    m_core;
    Device                                  m_device;

    std::array<Fence, FENCE_COUNT>          m_fences;
    std::array<GfxSemaphore, FENCE_COUNT>   m_semaphores;
    std::array<Pipeline*, PIPELINE_COUNT>   m_pipelines;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createPipelines();
    void    _createGfxSemaphores();
    void    _createFences();

    void    _destroyPipelines();
    void    _destroyGfxSemaphores();
    void    _destroyFences();

}; // class Renderer

} // namespace vox::gfx