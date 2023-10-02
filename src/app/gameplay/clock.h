
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clock.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 14:48:51 by etran             #+#    #+#             */
/*   Updated: 2023/09/29 14:48:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <chrono> // std::chrono

namespace vox {

class Clock {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using clock = std::chrono::high_resolution_clock;

	template<typename T>
	using duration = std::chrono::duration<T, std::chrono::seconds::period>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Clock() = default;

	Clock(Clock&& other) = delete;
	Clock(const Clock& other) = delete;
	Clock& operator=(Clock&& other) = delete;
	Clock& operator=(const Clock& other) = delete;

	/* ========================================================================= */

	float		get() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	const clock::time_point		_begin = clock::now();

}; // class Clock

} // namespace vox