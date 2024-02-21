/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:35:09 by etran             #+#    #+#             */
/*   Updated: 2024/02/21 11:18:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

App::App() {
}

App::~App() {
}

/* ========================================================================== */

void    App::run() {
    while (m_window.isAlive()) {
        m_window.pollEvents();
    }
}

} // namespace vox