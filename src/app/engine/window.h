/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:08 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 10:12:32 by etran            ###   ########.fr       */
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

	static constexpr const uint32_t		width = 800;
	static constexpr const uint32_t		height = 600;
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

	/* ========================================================================= */

	void							retrieveSize(int& width, int& height) const;
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

}; // class Window

} // namespace scop