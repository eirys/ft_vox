/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:56:28 by etran             #+#    #+#             */
/*   Updated: 2024/03/17 01:58:39 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.h"
#include "types.h"
#include "world.h"
#include "controller.h"

namespace game {

class GameState final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GameState() = default;

    void init(const ui::Window& window);
    void update(const ui::Window& window);

    World&                  getWorld() noexcept;
    const World&            getWorld() const noexcept;
    const ui::Controller&   getController() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    World           m_world;
    ui::Controller  m_controller;

};

} // namespace game