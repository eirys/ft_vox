/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 00:02:09 by etran             #+#    #+#             */
/*   Updated: 2024/03/12 11:15:46 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdexcept>

namespace ui {

static
void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

    const bool isPressed = action == GLFW_PRESS;
    switch (key) {
        case GLFW_KEY_ESCAPE:   if (isPressed) glfwSetWindowShouldClose(win, GLFW_TRUE); return;

        case GLFW_KEY_M:        if (isPressed) window->toggleMouse(); break;
        default:                break;
    }

    if (isPressed) window->toggleUpdate();
}

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Window::Window() {
    // initialize glfw
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    // disable OpenGL context creation
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // create a window pointer
    m_window = glfwCreateWindow(m_width, m_height, TITLE, nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("Failed to create window");

    // set pointer to window to `this` instance pointer
    // so we can access it from the callback functions
    glfwSetWindowUserPointer(m_window, this);
    // Setup event callbacks
    // glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    glfwSetKeyCallback(m_window, keyCallback);
    // glfwSetCursorPosCallback(m_window, cursorPositionCallback);

    // Disable cursor
    glfwSetInputMode(m_window, GLFW_CURSOR, m_mouseActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    // Remove glfw instance
    glfwTerminate();
}

/* ========================================================================== */

/**
 * @brief Freezes the program until the expected event occurs.
*/
void	Window::pause() const {
	// int	current_width, current_height;
	// retrieveSize(current_width, current_height);

	// while (current_width == 0 || current_height == 0) {
    while (true) {

		// retrieveSize(current_width, current_height);
    	glfwWaitEvents();
    }
	// }
}

/**
 * @brief Processes pending input events before returning.
*/
void    Window::pollEvents() const {
    glfwPollEvents();
}

/**
 * @brief Returns whether the window is still alive.
*/
bool    Window::isAlive() const {
    return !glfwWindowShouldClose(m_window);
}

void    Window::toggleMouse() noexcept {
    m_mouseActive = !m_mouseActive;
    glfwSetInputMode(m_window, GLFW_CURSOR, m_mouseActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void    Window::toggleUpdate() noexcept {
    m_isUpdated = !m_isUpdated;
}

void Window::retrieveFramebufferSize(int& width, int& height) const {
    glfwGetFramebufferSize(m_window, &width, &height);
}

/* ========================================================================== */

bool Window::isMouseActive() const noexcept {
    return m_mouseActive;
}

bool Window::needsUpdate() const noexcept {
    return m_isUpdated;
}

GLFWwindow* Window::getWindow() noexcept {
    return m_window;
}

GLFWwindow const* Window::getWindow() const noexcept {
    return m_window;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

} // namespace ui
