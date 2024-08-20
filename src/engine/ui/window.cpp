/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 00:02:09 by etran             #+#    #+#             */
/*   Updated: 2024/06/29 01:20:46 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include "controller.h"
#include "key_handler.h"
#include "debug.h"

#include <stdexcept>

namespace ui {

static constexpr u32 DEBUG_COUNT = 8;

static std::string debugWindow[DEBUG_COUNT] = {
    "none",
    "position",
    "depth",
    "normal",
    "albedo",
    "shadowmap",
    "ssao",
    "blur",
};

template<typename F>
static
void _setValue(KeyValueIndex index, F valueFunc) {
    KeyHandler::setKeyValue(index, valueFunc());
}

static
void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

    const bool isPressed = action == GLFW_PRESS;

    if (isPressed || action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_ESCAPE:       if (isPressed) glfwSetWindowShouldClose(win, GLFW_TRUE); return;

            // Switch
            case GLFW_KEY_M:            if (isPressed) KeyHandler::switchKey(KeySwitchIndex::MouseEnable); window->toggleMouse(); break;
            case GLFW_KEY_T:            if (isPressed) KeyHandler::switchKey(KeySwitchIndex::DisableTime); break;

            // Value
            case GLFW_KEY_O:
                if (isPressed)
                    _setValue(KeyValueIndex::DisplayDebug, [&](){
                        u32 updatedValue = (KeyHandler::getKeyValue(KeyValueIndex::DisplayDebug) + 1) % DEBUG_COUNT;
                        LINFO("Debug display: " << debugWindow[updatedValue]);
                        return updatedValue;
                    });
                break;

            // Toggle
            case GLFW_KEY_W:            isPressed ? KeyHandler::toggleKey(KeyToggleIndex::Forward) : KeyHandler::untoggleKey(KeyToggleIndex::Forward); break;
            case GLFW_KEY_S:            isPressed ? KeyHandler::toggleKey(KeyToggleIndex::Backward) : KeyHandler::untoggleKey(KeyToggleIndex::Backward); break;
            case GLFW_KEY_A:            isPressed ? KeyHandler::toggleKey(KeyToggleIndex::Left) : KeyHandler::untoggleKey(KeyToggleIndex::Left); break;
            case GLFW_KEY_D:            isPressed ? KeyHandler::toggleKey(KeyToggleIndex::Right) : KeyHandler::untoggleKey(KeyToggleIndex::Right); break;
            case GLFW_KEY_SPACE:        isPressed ? KeyHandler::toggleKey(KeyToggleIndex::Up) : KeyHandler::untoggleKey(KeyToggleIndex::Up); break;
            case GLFW_KEY_LEFT_CONTROL: isPressed ? KeyHandler::toggleKey(KeyToggleIndex::Down) : KeyHandler::untoggleKey(KeyToggleIndex::Down); break;
            case GLFW_KEY_R:            isPressed ? KeyHandler::toggleKey(KeyToggleIndex::Speed) : KeyHandler::untoggleKey(KeyToggleIndex::Speed); break;

            default:                    return;
        }
    }
}

static
void cursorPositionCallback(GLFWwindow* win, double xpos, double ypos) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

    if (Controller::isMouseActive())
        return;

    window->updateMousePos(xpos, ypos);
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
    toggleMouse();
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
    glfwSetInputMode(m_window, GLFW_CURSOR, Controller::isMouseActive() ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Window::retrieveFramebufferSize(int& width, int& height) const {
    glfwGetFramebufferSize(m_window, &width, &height);
}

void Window::updateMousePos(double x, double y) noexcept {
    m_mousePos = {x, y};
}

/* ========================================================================== */

const Window::MousePos& Window::getMousePos() const noexcept {
    return m_mousePos;
}

GLFWwindow* Window::getWindow() noexcept {
    return m_window;
}

GLFWwindow const* Window::getWindow() const noexcept {
    return m_window;
}

} // namespace ui
