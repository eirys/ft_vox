/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:46:03 by etran             #+#    #+#             */
/*   Updated: 2024/10/07 18:57:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_state.h"
#include "controller.h"
#include "maths.h"
#include "vertex_buffer.h"

#define TOGGLE_TIME 1

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

    m_camera.init(
        m_world.getOrigin() + math::vec3(0.0f, 5.0f, 0.0),
        math::vec3(0.0f, 0.0f, 1.0f));

    m_gameClock.init();
}

void GameState::update(const ui::Controller& controller) {
    // Camera
    if (!controller.isMouseActive()) {
        m_camera.update(controller);
    }

    // Sky
#if TOGGLE_TIME
    if (controller.isTimeEnabled()) {
        m_sun.update(m_gameClock.getElapsedTime());
    }
#endif

    // World
    m_world.update(m_camera);
    if (m_world.needsGfxUpdate()) {
    LINFO("Pos :" << m_camera.getChunkPosition());
    LINFO("offset :" << m_world.getPortionOffset());
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

const math::vec3& GameState::getSunPos() noexcept {
    return m_sun.m_direction;
}

const Camera& GameState::getCamera() noexcept {
    return m_camera;
}

} // namespace game