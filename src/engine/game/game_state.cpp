/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:46:03 by etran             #+#    #+#             */
/*   Updated: 2024/03/19 02:34:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_state.h"
#include "window.h"

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void GameState::init(const ui::Window& window) {
    m_world.init(42);
    m_controller.init(window);
    m_startTime = Clock::now();
}

/* ========================================================================== */

float GameState::getStartTime() const noexcept {
    return m_startTime.time_since_epoch().count();
}

float GameState::getElapsedTime() const noexcept {
    std::chrono::duration<float> elapsedTime = Clock::now() - m_startTime;
    return elapsedTime.count();
}

/* ========================================================================== */

void GameState::update(const ui::Window& window) {
    m_controller.update((window));
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

} // namespace game