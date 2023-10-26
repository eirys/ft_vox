/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 12:28:42 by eli               #+#    #+#             */
/*   Updated: 2023/08/10 22:06:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include "app.h"

namespace scop {

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

/**
 * @brief Function callback for window resize.
*/
static void	framebufferResizeCallback(
	GLFWwindow* window,
	int width,
	int height
) {
	(void)width;
	(void)height;

	Window*	handler = reinterpret_cast<Window*>(
		glfwGetWindowUserPointer(window)
	);
	handler->toggleFrameBufferResized(true);
}

/**
 * @brief Handle those keys only when focus on window.
*/
static void	handleKeyPressOnMouseUntoggled(int key, App* app) {
	if (key == GLFW_KEY_W) {
		return app->toggleMove(ObjectDirection::MOVE_FORWARD);
	} else if (key == GLFW_KEY_S) {
		return app->toggleMove(ObjectDirection::MOVE_BACKWARD);
	} else if (key == GLFW_KEY_A) {
		return app->toggleMove(ObjectDirection::MOVE_LEFT);
	} else if (key == GLFW_KEY_D) {
		return app->toggleMove(ObjectDirection::MOVE_RIGHT);
	} else if (key == GLFW_KEY_SPACE) {
		return app->toggleMove(ObjectDirection::MOVE_UP);
	} else if (key == GLFW_KEY_LEFT_CONTROL) {
		return app->toggleMove(ObjectDirection::MOVE_DOWN);
	}
}

/**
 * @brief Function callback for key press.
*/
static void	keyCallback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mods
) {
	(void)scancode;
	(void)mods;

	Window* win = reinterpret_cast<Window*>(
		glfwGetWindowUserPointer(window)
	);

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			return glfwSetWindowShouldClose(window, GLFW_TRUE);
		} else if (key == GLFW_KEY_TAB) {
			return win->toggleMouse();
		} else if (key == GLFW_KEY_R) {
			return win->getApp()->resetGame();
		} else if (!win->mouseActive()) {
			return handleKeyPressOnMouseUntoggled(key, win->getApp());
		}
	} else if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W) {
			return win->getApp()->untoggleMove(ObjectDirection::MOVE_FORWARD);
		} else if (key == GLFW_KEY_S) {
			return win->getApp()->untoggleMove(ObjectDirection::MOVE_BACKWARD);
		} else if (key == GLFW_KEY_A) {
			return win->getApp()->untoggleMove(ObjectDirection::MOVE_LEFT);
		} else if (key == GLFW_KEY_D) {
			return win->getApp()->untoggleMove(ObjectDirection::MOVE_RIGHT);
		} else if (key == GLFW_KEY_SPACE) {
			return win->getApp()->untoggleMove(ObjectDirection::MOVE_UP);
		} else if (key == GLFW_KEY_LEFT_CONTROL) {
			return win->getApp()->untoggleMove(ObjectDirection::MOVE_DOWN);
		}
	}
}

/**
 * @brief Function callback for cursor position.
*/
static void cursorPositionCallback(
	GLFWwindow* window,
	double xpos,
	double ypos
) {
	static float last_x = static_cast<float>(xpos);
	static float last_y = static_cast<float>(ypos);

	Window* win = reinterpret_cast<Window*>(
		glfwGetWindowUserPointer(window)
	);

	if (win->mouseActive()) {
		return;
	}

	win->getApp()->updateCameraDir(
		static_cast<float>(xpos) - last_x,
		last_y - static_cast<float>(ypos));

	last_x = static_cast<float>(xpos);
	last_y = static_cast<float>(ypos);
}

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Window::Window() {
	// initialize glfw
	glfwInit();

	// disable OpenGL context creation
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

Window::~Window() {
	if (_window != nullptr) {
		// Remove _window instance
		glfwDestroyWindow(_window);
	}
	// Remove glfw instance
	glfwTerminate();
}

/* ========================================================================== */

/**
 * @brief Initiate the window handle.
*/
void	Window::init(App* app_ptr) {
	_app = app_ptr;

	// create a window pointer
	_window = glfwCreateWindow(
		width,
		height,
		title,
		nullptr,
		nullptr
	);

	// set pointer to window to `this` instance pointer
	// so we can access it from the callback functions
	glfwSetWindowUserPointer(_window, this);

	// Setup event callbacks
	glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
	glfwSetKeyCallback(_window, keyCallback);
	glfwSetCursorPosCallback(_window, cursorPositionCallback);

	// Disable cursor
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/**
 * @brief Freezes the program until the window is resized.
*/
void	Window::pause() const {
	int	current_width, current_height;
	retrieveSize(current_width, current_height);

	while (current_width == 0 || current_height == 0) {
		retrieveSize(current_width, current_height);
		await();
	}
}

/**
 * @brief Processes pending input events before returning.
*/
void	Window::poll() const {
	glfwPollEvents();
}

/**
 * @brief Awaits for input.
*/
void	Window::await() const {
	glfwWaitEvents();
}

/**
 * @brief Returns whether the window is still alive.
*/
bool	Window::alive() const {
	return !glfwWindowShouldClose(_window);
}

/**
 * @brief Returns whether the window was resized.
*/
bool	Window::resized() const noexcept {
	return _frame_buffer_resized;
}

void	Window::toggleFrameBufferResized(bool is_resized) noexcept {
	_frame_buffer_resized = is_resized;
}

void	Window::toggleMouse() noexcept {
	_mouse_active = !_mouse_active;
	if (_mouse_active) {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

/* ========================================================================== */

void	Window::retrieveSize(int& width, int& height) const {
	glfwGetFramebufferSize(_window, &width, &height);
}

bool	Window::mouseActive() const noexcept {
	return _mouse_active;
}

GLFWwindow*	Window::getWindow() noexcept {
	return _window;
}

GLFWwindow const*	Window::getWindow() const noexcept {
	return _window;
}

App*	Window::getApp() noexcept {
	return _app;
}

App const*	Window::getApp() const noexcept {
	return _app;
}

} // namespace scop::graphics