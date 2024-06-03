/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:56:28 by etran             #+#    #+#             */
/*   Updated: 2024/05/17 16:59:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "world.h"
#include "controller.h"

#include <chrono>

namespace game {

struct Sun final {
    math::Vect3 m_direction;

    float       m_rotationSpeed = 0.1f * M_PI;
    bool        m_enabled = true;
};

class GameState final {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using Clock = std::chrono::high_resolution_clock;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GameState() = default;

    void init(const ui::Window& window);
    void update(const ui::Window& window);

    /* ====================================================================== */

    float getStartTime() const noexcept;
    float getElapsedTime() const noexcept;

    /* ====================================================================== */

    World&                  getWorld() noexcept;
    const World&            getWorld() const noexcept;
    const ui::Controller&   getController() const noexcept;
    const math::Vect3&      getSunPos() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    World               m_world;
    ui::Controller      m_controller;

    Sun                 m_sun;

    Clock::time_point   m_startTime;

};

} // namespace game