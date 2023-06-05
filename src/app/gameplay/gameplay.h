/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:49:39 by etran             #+#    #+#             */
/*   Updated: 2023/06/06 00:44:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "player.h"

namespace scop {
class App;
}

namespace vox {

class Gameplay {
public:

	friend scop::App;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Gameplay();

	~Gameplay() = default;

	Gameplay(Gameplay&& other) = delete;
	Gameplay(const Gameplay& other) = delete;
	Gameplay&	operator=(const Gameplay& other) = delete;

	/* ========================================================================= */

	void			setOrigin(const scop::Vect3& origin) noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::Vect3		world_origin = {0.0f, 0.0f, 0.0f};
	Player			player;

}; // class Gameplay

} // namespace vox