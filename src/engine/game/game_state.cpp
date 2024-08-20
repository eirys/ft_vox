/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:46:03 by etran             #+#    #+#             */
/*   Updated: 2024/08/15 16:41:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_state.h"
#include "controller.h"
#include "maths.h"

#define TOGGLE_TIME 0

#if TOGGLE_TIME
#define STABLE_TIME_CODE
#else
#define STABLE_TIME_CODE \
    float pos = M_PI * 0.25; \
 \
    m_sun.m_direction = math::Vect3( \
        std::cos(pos), \
        std::sin(pos), \
        0.0f); \
    return;
#endif

namespace game {

World   GameState::m_world;
Sun     GameState::m_sun;
Camera  GameState::m_camera;
Clock   GameState::m_gameClock;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void GameState::init(const ui::Controller& controller) {
    m_world.init();

    m_gameClock.init();
}

void GameState::update(const ui::Controller& controller) {
    if (!controller.isMouseActive()) {
        m_camera.update(controller);
    }

    STABLE_TIME_CODE

    if (controller.isTimeEnabled()) {
        const float time = m_gameClock.getElapsedTime();

        m_sun.m_direction = math::Vect3(
            std::cos(time * m_sun.m_rotationSpeed),
            std::sin(time * m_sun.m_rotationSpeed),
            0.0f);
    }
}

void GameState::ignoreData() {
    m_world.ignoreData();
}

/* ========================================================================== */

// TODO Fix
/**
 * @brief Returns the elapsed time between the beginning of the game and the last time the
 * game was updated.
 */
float GameState::getElapsedTime() noexcept {
    return m_gameClock.getElapsedTime();
}


const World& GameState::getWorld() noexcept {
    return m_world;
}

const math::Vect3& GameState::getSunPos() noexcept {
    return m_sun.m_direction;
}

const Camera& GameState::getCamera() noexcept {
    return m_camera;
}

} // namespace game