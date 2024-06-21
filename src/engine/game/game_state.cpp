/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:46:03 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 03:11:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_state.h"
#include "window.h"

#include "maths.h"

#define TOGGLE_TIME 1

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void GameState::init(const ui::Window& window) {
    m_world.init(42);
    m_controller.init(window);

    m_gameClock.init();
}

/* ========================================================================== */

// TODO Fix
/**
 * @brief Returns the elapsed time between the beginning of the game and the last time the
 * game was updated.
 */
float GameState::getElapsedTime() const noexcept {
    return m_gameClock.getElapsedTime();
}

/* ========================================================================== */

void GameState::update(const ui::Window& window) {
    m_controller.update((window));

#if !TOGGLE_TIME
    m_sun.m_direction = math::Vect3(
        std::cos(M_PI * 0.5),
        std::sin(M_PI * 0.5),
        0.0f);
    return;
#endif
    if (m_controller.isTimeEnabled()) {
        // If we reached the end of the day, we reset the time (and the pause time too)
        // if (m_gameClock.getElapsedTime() >= DAY_DURATION) {
        //     m_gameClock.init();
        // }

        const float time = m_gameClock.getElapsedTime();

        m_sun.m_direction = math::Vect3(
            std::cos(time * m_sun.m_rotationSpeed),
            std::sin(time * m_sun.m_rotationSpeed),
            0.0f);
    }
}

World& GameState::getWorld() noexcept {
    return m_world;
}

const World& GameState::getWorld() const noexcept {
    return m_world;
}

const ui::Controller& GameState::getController() const noexcept {
    return m_controller;
}

const math::Vect3& GameState::getSunPos() const noexcept {
    return m_sun.m_direction;
}

} // namespace game