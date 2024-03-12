/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:56:28 by etran             #+#    #+#             */
/*   Updated: 2024/03/12 10:45:39 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.h"
#include "types.h"

namespace vox::gfx {

class GameState final {
public:
    math::Vect3 color;
    u32         index = 0;

    void update() {
        constexpr math::Vect3 colors[2] = {
            {200.0f, 0.0f, 0.0f},
            {0.0f, 200.0f, 0.0f}
        };

        index = (index + 1) % 2;
        color = colors[index];
    }

};

}
