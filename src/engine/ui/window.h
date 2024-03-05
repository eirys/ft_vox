/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 23:59:08 by etran             #+#    #+#             */
/*   Updated: 2024/03/04 16:28:31 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <GLFW/glfw3.h>
# include <chrono>

# include "types.h"

namespace ui {

/**
 * @brief Window handler
*/
class Window final {
public:
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
    void    retrieveFramebufferSize(int& width, int& height) const;

    /* ========================================================================= */
    bool                isMouseActive() const noexcept;
    GLFWwindow*         getWindow() noexcept;
    GLFWwindow const*   getWindow() const noexcept;

private:
    /* ========================================================================= */
    /*                                    DATA                                   */
    /* ========================================================================= */

    GLFWwindow*     m_window = nullptr;
    u32             m_width = WIDTH;
    u32             m_height = HEIGHT;
    bool            m_mouseActive = true;

}; // class Window

} // namespace ui
