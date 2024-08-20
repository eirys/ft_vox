/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:15:33 by etran             #+#    #+#             */
/*   Updated: 2024/06/28 18:03:18 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "controller.h"
#include "window.h"
#include "renderer.h"
#include "game_state.h"
#include "timer.h"

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

    gfx::Renderer   m_renderer;
    ui::Window      m_window;
    ui::Timer       m_timer;

    ui::Controller  m_controller;
    game::GameState m_game;

}; // class Engine

} // namespace vox
