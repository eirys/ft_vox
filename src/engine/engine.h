/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:15:33 by etran             #+#    #+#             */
/*   Updated: 2024/05/31 02:19:05 by etran            ###   ########.fr       */
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

    ui::Window      m_window;
    gfx::Renderer   m_renderer;
    ui::Timer       m_timer;

    game::GameState m_game;

}; // class Engine

} // namespace vox
