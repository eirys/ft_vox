/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 23:59:08 by etran             #+#    #+#             */
/*   Updated: 2024/06/28 20:41:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <GLFW/glfw3.h>
# include <chrono>

# include "game_decl.h"
# include "types.h"
# include "enum.h"

namespace ui {

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

    static constexpr u32  WIDTH = WINDOW_WIDTH;
    static constexpr u32  HEIGHT = WINDOW_HEIGHT;
    static constexpr char TITLE[] = "FT_VOX";

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
    void    retrieveFramebufferSize(int& width, int& height) const;
    void    updateMousePos(double x, double y) noexcept;

    /* ========================================================================= */

    const MousePos&     getMousePos() const noexcept;
    GLFWwindow*         getWindow() noexcept;
    GLFWwindow const*   getWindow() const noexcept;

private:
    /* ========================================================================= */
    /*                                    DATA                                   */
    /* ========================================================================= */

    GLFWwindow*     m_window = nullptr;

    MousePos        m_mousePos = {0.0, (double)HEIGHT / 2.0};

    u32             m_width = WIDTH;
    u32             m_height = HEIGHT;

}; // class Window

} // namespace ui
