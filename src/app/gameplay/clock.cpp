/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clock.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 15:31:28 by etran             #+#    #+#             */
/*   Updated: 2023/09/29 15:31:28 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clock.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Returns elapsed time since begining of the game. Value is in seconds.
*/
float	Clock::get() const noexcept {
	return duration<float>(clock::now() - _begin).count();
}

} // namespace vox