/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:08 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 15:21:49 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Window handler
# include <GLFW/glfw3.h>

// Std
# include <string>
# include <chrono>

namespace scop {

class App;

/**
 * Window handler
*/
class Window {
public:
	/* ========================================================================= */
	/*                                  TYPEDEF                                  */
	/* ========================================================================= */

	typedef std::chrono::milliseconds	milliseconds;

	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr const uint32_t		width = 1200;
	static constexpr const uint32_t		height = 800;
	static constexpr const char			title[] = "FT_VOX";
	static constexpr const milliseconds	spam_delay = std::chrono::milliseconds(500);

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Window();
	~Window();

	Window(const Window& x) = delete;
	Window(Window&& x) = delete;
	Window& operator=(Window&& rhs) = delete;
	Window& operator=(const Window& rhs) = delete;

	/* ========================================================================= */

	void							init(App* app_ptr);

	void							pause() const;
	void							poll() const;
	void							await() const;
	bool							alive() const;
	bool							resized() const noexcept;
	void							toggleFrameBufferResized(bool resized) noexcept;
	void							toggleMouse() noexcept;

	/* ========================================================================= */

	void							retrieveSize(int& width, int& height) const;
	bool							mouseActive() const noexcept;
	GLFWwindow*						getWindow() noexcept;
	GLFWwindow const*				getWindow() const noexcept;
	App*							getApp() noexcept;
	App const*						getApp() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	GLFWwindow*						_window = nullptr;
	App*							_app = nullptr;
	bool							_frame_buffer_resized = false;
	bool							_mouse_active = false;

}; // class Window

} // namespace scop::graphics