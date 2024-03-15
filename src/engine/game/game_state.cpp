/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:46:03 by etran             #+#    #+#             */
/*   Updated: 2024/03/15 20:39:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_state.h"
#include "controller.h"

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

GameState::GameState() {
    m_world.init(42);
}

/* ========================================================================== */

void GameState::update(const ui::Controller& controller) {
    m_playerCamera = controller.computeView();
}

const math::Vect3& GameState::getPlayerCamera() const noexcept {
    return m_playerCamera;
}

World& GameState::getWorld() noexcept {
    return m_world;
}

const World& GameState::getWorld() const noexcept {
    return m_world;
}

} // namespace game