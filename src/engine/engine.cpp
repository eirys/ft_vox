/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:17:53 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 16:55:05 by etran            ###   ########.fr       */
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
    int i = 0;
    math::Vect3 colors[2] = {
        {200.0f, 0.0f, 0.0f},
        {0.0f, 200.0f, 0.0f}
    };

    while (m_window.isAlive()) {
        m_window.pollEvents();
        if (m_window.needsUpdate()) {
            i = (i + 1) % 2;
            m_game.color = colors[i];
            m_renderer.render(m_game);
            m_window.toggleUpdate();
        }
    }
    m_renderer.waitIdle();
}

} // namespace vox