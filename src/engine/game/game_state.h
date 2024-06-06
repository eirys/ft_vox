/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:56:28 by etran             #+#    #+#             */
/*   Updated: 2024/06/06 02:22:35 by etran            ###   ########.fr       */
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
};

struct Clock final {

    using Chrono = std::chrono::steady_clock;
    using Time = Chrono::time_point;

    void init() noexcept {
        m_start = Chrono::now();
    }

    float getElapsedTime() const noexcept {
        return std::chrono::duration<float>(Chrono::now() - m_start).count();
    }

    Time m_start;
};

class GameState final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GameState() = default;

    void init(const ui::Window& window);
    void update(const ui::Window& window);

    /* ====================================================================== */

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
    Clock               m_gameClock;

};

} // namespace game