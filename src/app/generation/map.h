/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 22:53:11 by etran             #+#    #+#             */
/*   Updated: 2023/07/16 15:23:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "vertex.h"

namespace vox {

/**
 * @brief Holds data about the map (cube positions, etc.).
*/
class Map {
public:
	/* ========================================================================= */
	/*                                HELPER CLASS                               */
	/* ========================================================================= */

	struct MapMesh {
		scop::Vect3					origin;
		std::vector<scop::Vertex>	vertices;
		std::vector<uint32_t>		indices;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Map();
	~Map();

	Map(Map&& src) = delete;
	Map(const Map& src) = delete;
	Map&operator=(Map&& rhs) = delete;
	Map&operator=(const Map& rhs) = delete;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	;

}; // class Map

} // namespace vox