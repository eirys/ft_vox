/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 20:35:46 by etran             #+#    #+#             */
/*   Updated: 2024/08/15 18:26:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controller.h"
#include "window.h"
#include "key_handler.h"
#include "camera.h"
#include "debug.h"

#include <algorithm>

namespace ui {

Controller::Settings Controller::ms_settings;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Controller::init(const Window& win) {
    const auto& mousePos = win.getMousePos();

    m_lastX = (float)mousePos.x;
    m_lastY = (float)mousePos.y;
}

void Controller::update(const Window& win) {
    ms_settings.ui.m_mouseActive = KeyHandler::isKeyOn(KeySwitchIndex::MouseEnable);

    if (isMouseActive())
        return;

    const auto& mousePos = win.getMousePos();
    const float deltaX = (float)mousePos.x - m_lastX;
    const float deltaY = m_lastY - (float)mousePos.y;

    m_lastX = (float)mousePos.x;
    m_lastY = (float)mousePos.y;

    m_yaw = std::fmod(std::fma(deltaX, game::Camera::getSettings().cameraSpeed, m_yaw), 360.0f);
    m_pitch = std::clamp(std::fma(deltaY, game::Camera::getSettings().cameraSpeed, m_pitch), -89.0f, 89.0f); // Clamp to avoid camera flipping.

    ms_settings.ui.m_isTimeEnabled = !KeyHandler::isKeyOn(KeySwitchIndex::DisableTime);
    ms_settings.ui.m_selectDebug = KeyHandler::getKeyValue(KeyValueIndex::DisplayDebug);
}

} // namespace ui