/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 20:35:46 by etran             #+#    #+#             */
/*   Updated: 2024/03/15 21:11:03 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controller.h"
#include "window.h"
#include "maths.h"
#include "debug.h"

namespace ui {
/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Controller::init(const Window& win) {
    const auto& mousePos = win.getMousePos();

    m_lastX = (float)mousePos.x;
    m_lastY = (float)mousePos.y;
}

void Controller::update(const Window& win) {
    if (win.isMouseActive())
        return;

    const auto& mousePos = win.getMousePos();
    const float deltaX = (float)mousePos.x - m_lastX;
    const float deltaY = m_lastY - (float)mousePos.y;

    constexpr float CAM_SPEED = 0.15f;

    const float camAngle = std::fma(deltaY, CAM_SPEED, m_pitch);
    m_yaw = std::fmod(std::fma(deltaX, CAM_SPEED, m_yaw), 360.0f);
    m_pitch = std::clamp(camAngle, -89.0f, 89.0f); // Clamp to avoid camera flipping.

    LDEBUG("Yaw: " << m_yaw);
    LDEBUG("Pitch: " << m_pitch);

    m_lastX = (float)mousePos.x;
    m_lastY = (float)mousePos.y;
}

math::Vect3 Controller::computeView() const {
    const float cosPitch = std::cos(math::radians(m_pitch));
    return math::Vect3(
        std::cos(math::radians(m_yaw)) * cosPitch,
        std::sin(math::radians(m_pitch)),
        std::sin(math::radians(m_yaw)) * cosPitch
    );
}

} // namespace ui