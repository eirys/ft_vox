/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 20:35:46 by etran             #+#    #+#             */
/*   Updated: 2024/06/15 11:32:44 by etran            ###   ########.fr       */
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

    m_camera.m_position = math::Vect3(WORLD_ORIGIN) + math::Vect3(0.0f, 20.0f, 0.0f);
    m_state.m_lastX = (float)mousePos.x;
    m_state.m_lastY = (float)mousePos.y;
}

void Controller::update(const Window& win) {
    if (win.isMouseActive())
        return;

    // Camera
    const auto& mousePos = win.getMousePos();
    const float deltaX = (float)mousePos.x - m_state.m_lastX;
    const float deltaY = m_state.m_lastY - (float)mousePos.y;

    constexpr float         CAM_SPEED = 0.15f;
    constexpr math::Vect3   UP_VEC = WORLD_Y;

    m_state.m_yaw = std::fmod(std::fma(deltaX, CAM_SPEED, m_state.m_yaw), 360.0f);
    m_state.m_pitch = std::clamp(std::fma(deltaY, CAM_SPEED, m_state.m_pitch), -89.0f, 89.0f); // Clamp to avoid camera flipping.

    LDEBUG("Yaw: " << m_state.m_yaw);
    LDEBUG("Pitch: " << m_state.m_pitch);

    const float yaw = math::radians(m_state.m_yaw);
    const float pitch = math::radians(m_state.m_pitch);

    const float cosPitch = std::cos(pitch);
    const float sinPitch = std::sin(pitch);
    const float cosYaw = std::cos(yaw);
    const float sinYaw = std::sin(yaw);

    m_camera.m_front = { cosYaw * cosPitch, sinPitch, sinYaw * cosPitch };
    m_camera.m_right = math::normalize(math::cross(m_camera.m_front, UP_VEC));
    m_camera.m_up = math::cross(m_camera.m_right, m_camera.m_front);

    m_state.m_lastX = (float)mousePos.x;
    m_state.m_lastY = (float)mousePos.y;

    // Position
    constexpr float NORMAL_SPEED = 0.15f;
    constexpr float HIGH_SPEED = 0.8f;

    const float moveSpeed = win.isKeyToggled(KeyToggleIndex::Speed) ? HIGH_SPEED : NORMAL_SPEED;

    if (win.isKeyToggled(KeyToggleIndex::Forward))
        m_camera.m_position += math::Vect3(cosYaw, 0.0f, sinYaw) * moveSpeed;
    else if (win.isKeyToggled(KeyToggleIndex::Backward))
        m_camera.m_position -= math::Vect3(cosYaw, 0.0f, sinYaw) * moveSpeed;

    if (win.isKeyToggled(KeyToggleIndex::Left))
        m_camera.m_position += math::Vect3(sinYaw, 0.0f, -cosYaw) * moveSpeed;
    else if (win.isKeyToggled(KeyToggleIndex::Right))
        m_camera.m_position -= math::Vect3(sinYaw, 0.0f, -cosYaw) * moveSpeed;

    if (win.isKeyToggled(KeyToggleIndex::Up))
        m_camera.m_position.y += moveSpeed;
    else if (win.isKeyToggled(KeyToggleIndex::Down))
        m_camera.m_position.y -= moveSpeed;

    m_isTimeEnabled = !win.isKeyOn(KeySwitchIndex::DisableTime);
    m_selectDebug = win.getKeyValue(KeyValueIndex::DisplayDebug);
}

const Camera& Controller::getCamera() const noexcept {
    return m_camera;
}

u32 Controller::showDebug() const noexcept {
    return m_selectDebug;
}

bool Controller::isTimeEnabled() const noexcept {
    return m_isTimeEnabled;
}

} // namespace ui