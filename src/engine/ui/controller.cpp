/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 20:35:46 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 22:23:41 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controller.h"
#include "window.h"
#include "maths.h"
#include "debug.h"

#include <algorithm>

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

    constexpr float         CAM_SPEED = 0.15f;
    constexpr math::Vect3   UP_VEC = {0.0f, 1.0f, 0.0f};

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

    m_camera.m_front = { cosYaw * cosPitch, sinPitch, sinYaw * cosPitch };
    m_camera.m_right = math::normalize(math::cross(m_camera.m_front, UP_VEC));
    m_camera.m_up = math::cross(m_camera.m_right, m_camera.m_front);

    m_lastX = (float)mousePos.x;
    m_lastY = (float)mousePos.y;

    // Position
    constexpr float NORMAL_SPEED = 0.15f;
    constexpr float HIGH_SPEED = 0.6f;

    const float moveSpeed = win.isKeyPressed(ControlKeyIndex::Speed) ? HIGH_SPEED : NORMAL_SPEED;

    if (win.isKeyPressed(ControlKeyIndex::Forward))
        m_camera.m_position += math::Vect3(cosYaw, 0.0f, sinYaw) * moveSpeed;
    else if (win.isKeyPressed(ControlKeyIndex::Backward))
        m_camera.m_position -= math::Vect3(cosYaw, 0.0f, sinYaw) * moveSpeed;

    if (win.isKeyPressed(ControlKeyIndex::Left))
        m_camera.m_position += math::Vect3(sinYaw, 0.0f, -cosYaw) * moveSpeed;
    else if (win.isKeyPressed(ControlKeyIndex::Right))
        m_camera.m_position -= math::Vect3(sinYaw, 0.0f, -cosYaw) * moveSpeed;

    if (win.isKeyPressed(ControlKeyIndex::Up))
        m_camera.m_position.y += moveSpeed;
    else if (win.isKeyPressed(ControlKeyIndex::Down))
        m_camera.m_position.y -= moveSpeed;
}

const Camera& Controller::getCamera() const noexcept {
    return m_camera;
}

} // namespace ui