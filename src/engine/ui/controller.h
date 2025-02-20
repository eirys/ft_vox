/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 19:33:01 by etran             #+#    #+#             */
/*   Updated: 2024/09/17 14:52:28 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.h"
#include "types.h"

#include "game_decl.h"

namespace ui {

class Window;

/**
 * @brief Player controller
 */
class Controller final {
public:
    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct Settings {
        struct UI {
            u32     m_selectDebug = 0;
            bool    m_isTimeEnabled = true;
            bool    m_mouseActive = false;
        }   ui;

        struct Gfx {
            u32     m_fogDistance = 100;
        }   gfx;

        bool updated = true;
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Controller() = default;
    ~Controller() = default;

    Controller(Controller&& x) = delete;
    Controller(const Controller& x) = delete;
    Controller& operator=(Controller&& rhs) = delete;
    Controller& operator=(const Controller& rhs) = delete;

    /* ====================================================================== */

    void init(const Window& win);
    void update(const Window& win);

    /* ====================================================================== */

    static bool     isTimeEnabled() noexcept { return ms_settings.ui.m_isTimeEnabled; }
    static u32      showDebug() noexcept { return ms_settings.ui.m_selectDebug; }
    static bool     isMouseActive() noexcept { return ms_settings.ui.m_mouseActive; }

    static u32      getFogDistance() noexcept { return ms_settings.gfx.m_fogDistance; }
    static bool     settingsUpdated() noexcept { return ms_settings.updated; }
    static void     unsetSettingsUpdated() noexcept { ms_settings.updated = false; }

    f32     getYaw() const noexcept { return m_yaw; }
    f32     getPitch() const noexcept { return m_pitch; }

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static Settings ms_settings;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    math::vec2      m_lastMousePos = math::vec2(0.0f, 0.0f);

    f32             m_yaw = 0;
    f32             m_pitch = 0;

}; // class Controller

} // namespace ui