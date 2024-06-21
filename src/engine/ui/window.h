/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 23:59:08 by etran             #+#    #+#             */
/*   Updated: 2024/06/15 11:35:10 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <GLFW/glfw3.h>
# include <chrono>

# include "types.h"
# include "enum.h"

namespace ui {

/**
 * @brief Keys meant to be maintained to be activated.
 */
enum class KeyToggleIndex {
    // Movement
    Forward = 0,
    Backward,
    Left,
    Right,
    Up,
    Down,
    Speed,

    Count
};

/**
 * @brief Keys meant to be switched on/off.
 */
enum class KeySwitchIndex {
    DisableTime,

    Count
};

/**
 * @brief Keys meant to be pressed a couple time to select a value.
 */
enum class KeyValueIndex {
    DisplayDebug,

    Count
};

constexpr u32 TOGGLE_INDEX_COUNT = (u32)KeyToggleIndex::Count;
constexpr u32 SWITCH_INDEX_COUNT = (u32)KeySwitchIndex::Count;
constexpr u32 VALUE_INDEX_COUNT = (u32)KeyValueIndex::Count;

/**
 * @brief Window handler
*/
class Window final {
public:
    /* ====================================================================== */
    /*                             HELPER OBJECTS                             */
    /* ====================================================================== */

    struct MousePos {
        double x;
        double y;
    };

    /* ========================================================================= */
    /*                                  TYPEDEF                                  */
    /* ========================================================================= */

    using ms = std::chrono::milliseconds;

    /* ========================================================================= */
    /*                               CONST MEMBERS                               */
    /* ========================================================================= */

    static constexpr u32  WIDTH = 1200;
    static constexpr u32  HEIGHT = 800;
    static constexpr char TITLE[] = "FT_VOX";
    static constexpr ms   SPAM_DELAY = ms(500);

    /* ========================================================================= */
    /*                                  METHODS                                  */
    /* ========================================================================= */

    Window();
    ~Window();

    Window(Window&& x) = delete;
    Window(const Window& x) = delete;
    Window& operator=(Window&& rhs) = delete;
    Window& operator=(const Window& rhs) = delete;

    /* ========================================================================= */

    void    pause() const;
    void    pollEvents() const;
    bool    isAlive() const;
    // bool    resized() const noexcept;
    // void    toggleFrameBufferResized(bool resized) noexcept;
    void    toggleMouse() noexcept;
    void    toggleUpdate() noexcept;
    void    retrieveFramebufferSize(int& width, int& height) const;
    void    updateMousePos(double x, double y) noexcept;

    /* ====================================================================== */

    void    toggleKey(const KeyToggleIndex index) noexcept;
    void    untoggleKey(const KeyToggleIndex index) noexcept;
    bool    isKeyToggled(const KeyToggleIndex index) const noexcept;

    void    switchKey(const KeySwitchIndex index) noexcept;
    bool    isKeyOn(const KeySwitchIndex index) const noexcept;

    void    setKeyValue(const KeyValueIndex index, const u32 value) noexcept;
    u32     getKeyValue(const KeyValueIndex index) const noexcept;

    /* ========================================================================= */

    const MousePos&     getMousePos() const noexcept;
    bool                isMouseActive() const noexcept;
    bool                needsUpdate() const noexcept;
    GLFWwindow*         getWindow() noexcept;
    GLFWwindow const*   getWindow() const noexcept;

private:
    /* ========================================================================= */
    /*                                    DATA                                   */
    /* ========================================================================= */

    GLFWwindow*     m_window = nullptr;

    bool            m_toggledKeys[TOGGLE_INDEX_COUNT] = { false };
    bool            m_switchedKeys[SWITCH_INDEX_COUNT] = { false };
    u32             m_keyValue[VALUE_INDEX_COUNT] = { 0 };

    MousePos        m_mousePos = {0.0, (double)HEIGHT / 2.0};

    u32             m_width = WIDTH;
    u32             m_height = HEIGHT;
    bool            m_mouseActive = false;
    bool            m_isUpdated = true;

}; // class Window

} // namespace ui
