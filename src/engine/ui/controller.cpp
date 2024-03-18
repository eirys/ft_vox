/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 20:35:46 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 11:44:06 by etran            ###   ########.fr       */
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

    // Camera
    const auto& mousePos = win.getMousePos();
    const float deltaX = (float)mousePos.x - m_lastX;
    const float deltaY = m_lastY - (float)mousePos.y;

    constexpr float CAM_SPEED = 0.15f;

    m_yaw = std::fmod(std::fma(deltaX, CAM_SPEED, m_yaw), 360.0f);
    m_pitch = std::clamp(std::fma(deltaY, CAM_SPEED, m_pitch), -89.0f, 89.0f); // Clamp to avoid camera flipping.

    LDEBUG("Yaw: " << m_yaw);
    LDEBUG("Pitch: " << m_pitch);

    const float yaw = math::radians(m_yaw);
    const float pitch = math::radians(m_pitch);

    const float cosPitch = std::cos(pitch);
    const float sinPitch = std::sin(pitch);
    const float cosYaw = std::cos(yaw);
    const float sinYaw = std::sin(yaw);

    m_direction = { cosYaw * cosPitch, sinPitch, sinYaw * cosPitch };

    m_lastX = (float)mousePos.x;
    m_lastY = (float)mousePos.y;

    // Position
    constexpr float     MOVE_SPEED = 0.1f;

    if (win.isKeyPressed(KeyIndex::Forward))
        m_position += math::Vect3(cosYaw, 0.0f, sinYaw) * MOVE_SPEED;
    else if (win.isKeyPressed(KeyIndex::Backward))
        m_position -= math::Vect3(cosYaw, 0.0f, sinYaw) * MOVE_SPEED;

    if (win.isKeyPressed(KeyIndex::Left))
        m_position += math::Vect3(sinYaw, 0.0f, -cosYaw) * MOVE_SPEED;
    else if (win.isKeyPressed(KeyIndex::Right))
        m_position -= math::Vect3(sinYaw, 0.0f, -cosYaw) * MOVE_SPEED;

    if (win.isKeyPressed(KeyIndex::Up))
        m_position.y += MOVE_SPEED;
    else if (win.isKeyPressed(KeyIndex::Down))
        m_position.y -= MOVE_SPEED;
}

const math::Vect3& Controller::getView() const {
    return m_direction;
}

const math::Vect3& Controller::getPosition() const noexcept {
    return m_position;
}

} // namespace ui