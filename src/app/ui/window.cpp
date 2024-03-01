/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 00:02:09 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 01:04:30 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdexcept>

namespace ui {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Window::Window() {
    // initialize glfw
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    // disable OpenGL context creation
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // create a window pointer
    m_window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!m_window)
        throw std::runtime_error("Failed to create window");

    // set pointer to window to `this` instance pointer
    // so we can access it from the callback functions
    glfwSetWindowUserPointer(m_window, this);

    // Setup event callbacks
    // glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    // glfwSetKeyCallback(m_window, keyCallback);
    // glfwSetCursorPosCallback(m_window, cursorPositionCallback);

    // Disable cursor
    glfwSetInputMode(m_window, GLFW_CURSOR, m_mouseActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

Window::~Window() {
    if (m_window != nullptr) {
        glfwDestroyWindow(m_window);
    }
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

/* ========================================================================== */

bool Window::isMouseActive() const noexcept {
    return m_mouseActive;
}

GLFWwindow* Window::getWindow() noexcept {
    return m_window;
}

GLFWwindow const* Window::getWindow() const noexcept {
    return m_window;
}


} // namespace ui
