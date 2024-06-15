/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 00:02:09 by etran             #+#    #+#             */
/*   Updated: 2024/06/15 01:48:27 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdexcept>

namespace ui {

static
void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

    const bool isPressed = action == GLFW_PRESS;

    if (isPressed || action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_ESCAPE:       if (isPressed) glfwSetWindowShouldClose(win, GLFW_TRUE); return;

            case GLFW_KEY_M:            if (isPressed) window->toggleMouse(); break;

            // Debug
            case GLFW_KEY_O:            if (isPressed) window->switchKey(ControlKeyIndex::DisplayDebug); break;
            case GLFW_KEY_T:            if (isPressed) window->switchKey(ControlKeyIndex::DisableTime); break;

            case GLFW_KEY_RIGHT:        if (isPressed) window->m_selectedValue = (window->m_selectedValue + 1) % 4; break;
            case GLFW_KEY_LEFT:         if (isPressed) window->m_selectedValue = (window->m_selectedValue + (4 - 1)) % 4; break;

            // Movement
            case GLFW_KEY_W:            isPressed ? window->toggleKey(ControlKeyIndex::Forward) : window->untoggleKey(ControlKeyIndex::Forward); break;
            case GLFW_KEY_S:            isPressed ? window->toggleKey(ControlKeyIndex::Backward) : window->untoggleKey(ControlKeyIndex::Backward); break;
            case GLFW_KEY_A:            isPressed ? window->toggleKey(ControlKeyIndex::Left) : window->untoggleKey(ControlKeyIndex::Left); break;
            case GLFW_KEY_D:            isPressed ? window->toggleKey(ControlKeyIndex::Right) : window->untoggleKey(ControlKeyIndex::Right); break;
            case GLFW_KEY_SPACE:        isPressed ? window->toggleKey(ControlKeyIndex::Up) : window->untoggleKey(ControlKeyIndex::Up); break;
            case GLFW_KEY_LEFT_CONTROL: isPressed ? window->toggleKey(ControlKeyIndex::Down) : window->untoggleKey(ControlKeyIndex::Down); break;
            case GLFW_KEY_R:            isPressed ? window->toggleKey(ControlKeyIndex::Speed) : window->untoggleKey(ControlKeyIndex::Speed); break;

            default:                    break;
        }

        // window->toggleUpdate();
    }
}

static
void cursorPositionCallback(GLFWwindow* win, double xpos, double ypos) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

    if (window->isMouseActive())
        return;

    window->updateMousePos(xpos, ypos);
    // window->toggleUpdate();
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
    glfwSetCursorPosCallback(m_window, cursorPositionCallback);

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

void Window::updateMousePos(double x, double y) noexcept {
    m_mousePos = {x, y};
}

void Window::toggleKey(const ControlKeyIndex ControlKeyIndex) noexcept {
    if (isKeyPressed(ControlKeyIndex))
        return;
    m_pressedKeys[(u32)ControlKeyIndex] = true;
}

void Window::untoggleKey(const ControlKeyIndex ControlKeyIndex) noexcept {
    if (!isKeyPressed(ControlKeyIndex))
        return;
    m_pressedKeys[(u32)ControlKeyIndex] = false;
}

void Window::switchKey(const ControlKeyIndex ControlKeyIndex) noexcept {
    m_pressedKeys[(u32)ControlKeyIndex] = !m_pressedKeys[(u32)ControlKeyIndex];
}

/* ========================================================================== */

const Window::MousePos& Window::getMousePos() const noexcept {
    return m_mousePos;
}

bool Window::isKeyPressed(const ControlKeyIndex ControlKeyIndex) const noexcept {
    return m_pressedKeys[(u32)ControlKeyIndex];
}

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

} // namespace ui
