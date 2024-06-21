/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 00:02:09 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 17:06:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include <stdexcept>

#include "debug.h"

namespace ui {

static constexpr u32 DEBUG_COUNT = 9;

static
std::string currentDebug(u32 i) {
    static std::string debugWindow[DEBUG_COUNT] = {
        "none",
        "position",
        "depth",
        "normal",
        "albedo",
        "shadowmap",
        "ssao",
        "blur",
        "inverse normal"
    };
    return debugWindow[i];
}

static
void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

    const bool isPressed = action == GLFW_PRESS;

    if (isPressed || action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_ESCAPE:       if (isPressed) glfwSetWindowShouldClose(win, GLFW_TRUE); return;

            case GLFW_KEY_M:            if (isPressed) window->toggleMouse(); break;

            // Switch
            case GLFW_KEY_T:            if (isPressed) window->switchKey(KeySwitchIndex::DisableTime); break;

            // Value
            case GLFW_KEY_O:
                if (isPressed) {
                    u32 updatedValue = window->getKeyValue(KeyValueIndex::DisplayDebug);
                    updatedValue = (updatedValue + 1) % DEBUG_COUNT;
                    window->setKeyValue(KeyValueIndex::DisplayDebug, updatedValue);
                    LINFO("Debug display: " << currentDebug(updatedValue));
                    break;
                }

            // Toggle
            case GLFW_KEY_W:            isPressed ? window->toggleKey(KeyToggleIndex::Forward) : window->untoggleKey(KeyToggleIndex::Forward); break;
            case GLFW_KEY_S:            isPressed ? window->toggleKey(KeyToggleIndex::Backward) : window->untoggleKey(KeyToggleIndex::Backward); break;
            case GLFW_KEY_A:            isPressed ? window->toggleKey(KeyToggleIndex::Left) : window->untoggleKey(KeyToggleIndex::Left); break;
            case GLFW_KEY_D:            isPressed ? window->toggleKey(KeyToggleIndex::Right) : window->untoggleKey(KeyToggleIndex::Right); break;
            case GLFW_KEY_SPACE:        isPressed ? window->toggleKey(KeyToggleIndex::Up) : window->untoggleKey(KeyToggleIndex::Up); break;
            case GLFW_KEY_LEFT_CONTROL: isPressed ? window->toggleKey(KeyToggleIndex::Down) : window->untoggleKey(KeyToggleIndex::Down); break;
            case GLFW_KEY_R:            isPressed ? window->toggleKey(KeyToggleIndex::Speed) : window->untoggleKey(KeyToggleIndex::Speed); break;

            default:                    return;
        }
    }
}

static
void cursorPositionCallback(GLFWwindow* win, double xpos, double ypos) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

    if (window->isMouseActive())
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

void Window::retrieveFramebufferSize(int& width, int& height) const {
    glfwGetFramebufferSize(m_window, &width, &height);
}

void Window::updateMousePos(double x, double y) noexcept {
    m_mousePos = {x, y};
}

/* ========================================================================== */

void Window::toggleKey(const KeyToggleIndex index) noexcept {
    if (isKeyToggled(index))
        return;
    m_toggledKeys[(u32)index] = true;
}

void Window::untoggleKey(const KeyToggleIndex index) noexcept {
    if (!isKeyToggled(index))
        return;
    m_toggledKeys[(u32)index] = false;
}
bool Window::isKeyToggled(const KeyToggleIndex index) const noexcept {
    return m_toggledKeys[(u32)index];
}

void Window::switchKey(const KeySwitchIndex index) noexcept {
    m_switchedKeys[(u32)index] = !m_switchedKeys[(u32)index];
}

bool Window::isKeyOn(const KeySwitchIndex index) const noexcept {
    return m_switchedKeys[(u32)index];
}

void Window::setKeyValue(const KeyValueIndex index, const u32 value) noexcept {
    m_keyValue[(u32)index] = value;
}

u32 Window::getKeyValue(const KeyValueIndex index) const noexcept {
    return m_keyValue[(u32)index];
}

/* ========================================================================== */

const Window::MousePos& Window::getMousePos() const noexcept {
    return m_mousePos;
}


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
