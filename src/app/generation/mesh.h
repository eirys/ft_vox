/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:21:11 by etran             #+#    #+#             */
/*   Updated: 2023/09/27 14:21:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector> // std::vector

namespace vox {

struct Vertex;

struct Mesh {

	std::vector<Vertex>		vertices;
	std::vector<uint32_t>	indices;

}; // struct Mesh

} // namespace vox