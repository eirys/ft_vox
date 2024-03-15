/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:56:28 by etran             #+#    #+#             */
/*   Updated: 2024/03/15 21:00:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.h"
#include "types.h"
#include "world.h"

namespace ui {
class Controller;
}

namespace game {

class GameState final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GameState();

    void update(const ui::Controller& controller);

    World&              getWorld() noexcept;
    const World&        getWorld() const noexcept;
    const math::Vect3&  getPlayerCamera() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    World       m_world;
    math::Vect3 m_playerCamera = {0.0f, 0.0f, 1.0f};

};

} // namespace game