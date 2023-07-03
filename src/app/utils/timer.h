/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 21:13:23 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 11:52:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <chrono> // std::chrono::time_point
# include <iostream> // std::cout, std::endl

namespace scop {

/**
 * @brief FPS counter.
*/
class Timer {
public:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static const constexpr int	log_interval = 2; // s

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Timer() = default;
	~Timer() = default;

	Timer(Timer&& other) = delete;
	Timer(const Timer& other) = delete;
	Timer&	operator=(Timer&& other) = delete;
	Timer&	operator=(const Timer& other) = delete;

	/* ========================================================================= */

	/**
	 * @brief Starts the timer.
	*/
	void	start() noexcept {
		begin = clock::now();
		frames = 0;
	}

	/**
	 * @brief Updates the frame counter.
	*/
	void	update() noexcept {
		++frames;
	}

	/**
	 * @brief Counts the number of fps. Logged every log_interval second.
	*/
	void	check() {
		clock::time_point	now = clock::now();

		if (now - begin >= std::chrono::seconds(log_interval)) {
			float fps = frames / log_interval;
			frames = 0;
			begin = now;
			std::cout << fps << " fps" << std::endl;
		}
	}

private:
	/* ========================================================================= */
	/*                                  TYPEDEF                                  */
	/* ========================================================================= */

	typedef	std::chrono::high_resolution_clock	clock;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	clock::time_point		begin;
	std::size_t				frames;

}; // class Timer

} // namespace scop