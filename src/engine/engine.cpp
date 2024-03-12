/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:17:53 by etran             #+#    #+#             */
/*   Updated: 2024/03/12 10:45:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

#include "debug.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Engine::Engine() {
    m_renderer.init(m_window, m_game);

    LINFO("Engine initialized.");
}

Engine::~Engine() {
    m_renderer.destroy();

    LINFO("Engine destroyed.");
}

/* ========================================================================== */

void Engine::run() {
    while (m_window.isAlive()) {
        m_window.pollEvents();
        if (m_window.needsUpdate()) {
            m_game.update();
            m_renderer.render(m_game);
            m_window.toggleUpdate();
        }
    }
    m_renderer.waitIdle();
}

} // namespace vox