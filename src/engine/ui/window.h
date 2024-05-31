/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 23:59:08 by etran             #+#    #+#             */
/*   Updated: 2024/05/31 13:07:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <GLFW/glfw3.h>
# include <chrono>

# include "types.h"
# include "enum.h"

namespace ui {

enum class ControlKeyIndex {
    Forward = 0,
    Backward,
    Left,
    Right,
    Up,
    Down,
    Speed,

    First = Forward,
    Last = Speed
};

constexpr u32 CONTROL_KEY_COUNT = enumSize<ControlKeyIndex>();

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
    void    toggleKey(const ControlKeyIndex ControlKeyIndex) noexcept;
    void    untoggleKey(const ControlKeyIndex ControlKeyIndex) noexcept;
    void    retrieveFramebufferSize(int& width, int& height) const;
    void    updateMousePos(double x, double y) noexcept;

    /* ========================================================================= */
    const MousePos&     getMousePos() const noexcept;
    bool                isKeyPressed(const ControlKeyIndex ControlKeyIndex) const noexcept;
    bool                isMouseActive() const noexcept;
    bool                needsUpdate() const noexcept;
    GLFWwindow*         getWindow() noexcept;
    GLFWwindow const*   getWindow() const noexcept;

private:
    /* ========================================================================= */
    /*                                    DATA                                   */
    /* ========================================================================= */

    GLFWwindow*     m_window = nullptr;

    bool            m_pressedKeys[CONTROL_KEY_COUNT] = { false };
    MousePos        m_mousePos = {0.0, (double)HEIGHT / 2.0};

    u32             m_width = WIDTH;
    u32             m_height = HEIGHT;
    bool            m_mouseActive = false;
    bool            m_isUpdated = true;

}; // class Window

} // namespace ui
