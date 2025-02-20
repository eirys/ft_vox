/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:17:53 by etran             #+#    #+#             */
/*   Updated: 2024/09/11 15:51:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

#include "debug.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Engine::Engine() {
    m_controller.init(m_window);
    m_game.init(m_controller);
    m_renderer.init(m_window);

    m_game.ignoreData();

    LINFO("Engine initialized.");
}

Engine::~Engine() {
    m_renderer.destroy();

    LINFO("Engine destroyed.");
}

/* ========================================================================== */

void Engine::run() {
    m_timer.init();

    while (m_window.isAlive()) {
        m_window.pollEvents();

        m_controller.update(m_window);
        m_game.update(m_controller);

        m_renderer.render();
        m_timer.update();
    }

    m_renderer.waitIdle();
}

} // namespace vox