/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 12:28:42 by eli               #+#    #+#             */
/*   Updated: 2023/06/06 00:18:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"
#include "app.h"

namespace scop {

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

/**
 * Function callback for window resize
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
 * Toggles texture on/off
*/
static void	toggleTextureCallback(
	App* app_ptr
) noexcept {
	using std::chrono::steady_clock;

	static steady_clock::time_point	key_pressed{};

	steady_clock::time_point	now = steady_clock::now();
	steady_clock::duration	duration =
		std::chrono::duration_cast<Window::milliseconds>(
			now - key_pressed
		);

	// Waits for the color transition to finish
	if (duration < Window::spam_delay) {
		return;
	}
	app_ptr->toggleTexture();

	// Update last key press
	key_pressed = now;
}

/**
 * Function callback for key press
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
		} else if (key == GLFW_KEY_T) {
			return toggleTextureCallback(win->getApp());
		} else if (key == GLFW_KEY_L) {
			return win->getApp()->toggleLightPos();
		} else if (key == GLFW_KEY_K) {
			return win->getApp()->toggleLightColor();
		} else if (key == GLFW_KEY_W) {
			return win->getApp()->toggleMove(ObjectDirection::MOVE_FORWARD);
		} else if (key == GLFW_KEY_S) {
			return win->getApp()->toggleMove(ObjectDirection::MOVE_BACKWARD);
		} else if (key == GLFW_KEY_A) {
			return win->getApp()->toggleMove(ObjectDirection::MOVE_LEFT);
		} else if (key == GLFW_KEY_D) {
			return win->getApp()->toggleMove(ObjectDirection::MOVE_RIGHT);
		} else if (key == GLFW_KEY_SPACE) {
			return win->getApp()->toggleMove(ObjectDirection::MOVE_UP);
		} else if (key == GLFW_KEY_LEFT_CONTROL) {
			return win->getApp()->toggleMove(ObjectDirection::MOVE_DOWN);
		} else if (key == GLFW_KEY_R) {
			return win->getApp()->reset();
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

static void cursorPositionCallback(
	GLFWwindow* window,
	double xpos,
	double ypos
) {
	static float last_x = xpos;
	static float last_y = ypos;

	Window* win = reinterpret_cast<Window*>(
		glfwGetWindowUserPointer(window)
	);
	win->getApp()->updateCameraDir(xpos - last_x, last_y - ypos);

	last_x = xpos;
	last_y = ypos;
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
	if (window != nullptr) {
		// Remove window instance
		glfwDestroyWindow(window);
	}
	// Remove glfw instance
	glfwTerminate();
}

/* ========================================================================== */

/**
 * @brief Initiate the window handle.
*/
void	Window::init(App* app_ptr) {
	app = app_ptr;

	// create a window pointer
	window = glfwCreateWindow(
		width,
		height,
		title,
		nullptr,
		nullptr
	);

	// set pointer to window to `this` instance pointer
	// so we can access it from the callback functions
	glfwSetWindowUserPointer(window, this);

	// Setup event callbacks
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);

	// Disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	return !glfwWindowShouldClose(window);
}

/**
 * @brief Returns whether the window was resized.
*/
bool	Window::resized() const noexcept {
	return frame_buffer_resized;
}

/* ========================================================================== */

void	Window::retrieveSize(int& width, int& height) const {
	glfwGetFramebufferSize(window, &width, &height);
}

GLFWwindow*	Window::getWindow() noexcept {
	return window;
}

GLFWwindow const*	Window::getWindow() const noexcept {
	return window;
}

App*	Window::getApp() noexcept {
	return app;
}

App const*	Window::getApp() const noexcept {
	return app;
}

void	Window::toggleFrameBufferResized(bool is_resized) noexcept {
	frame_buffer_resized = is_resized;
}

} // namespace scop