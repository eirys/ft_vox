/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:17:21 by etran             #+#    #+#             */
/*   Updated: 2024/02/27 18:17:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core.h"
#include "device.h"

namespace vox::gfx {

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

    void init();
    void destroy();

    void render();

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Core      m_core;
    Device    m_device;

}; // class Renderer

} // namespace vox::gfx