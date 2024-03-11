/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:15:33 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 16:57:37 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "window.h"
#include "renderer.h"
#include "game_state.h"

namespace vox {

class Engine final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Engine();
    ~Engine();

    Engine(Engine&& other) = delete;
    Engine(const Engine& other) = delete;
    Engine& operator=(Engine&& other) = delete;
    Engine& operator=(const Engine& other) = delete;

    /* ====================================================================== */

    void    run();

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    ui::Window      m_window;
    // ui::Controller  m_controller;
    gfx::Renderer   m_renderer;

    gfx::GameState  m_game;

}; // class Engine

} // namespace vox
