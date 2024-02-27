/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:15:33 by etran             #+#    #+#             */
/*   Updated: 2024/02/27 18:17:35 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "renderer.h"

namespace vox {

class Engine final {
public:

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    gfx::Renderer    m_renderer;

}; // class Engine

} // namespace vox
