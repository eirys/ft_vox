/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:56:28 by etran             #+#    #+#             */
/*   Updated: 2024/09/17 14:27:31 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "world.h"
#include "camera.h"
#include "sun.h"

#include <chrono>

namespace game {

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
    ~GameState() = default;

    /* ====================================================================== */

    void init(const ui::Controller& controller);
    void update(const ui::Controller& controller);

    void ignoreData();

    /* ====================================================================== */

    static float                getElapsedTime() noexcept;
    static const World&         getWorld() noexcept;
    static const math::vec3&    getSunPos() noexcept;
    static const Camera&        getCamera() noexcept;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static World   m_world;
    static Sun     m_sun;
    static Camera  m_camera;
    static Clock   m_gameClock;

};

} // namespace game