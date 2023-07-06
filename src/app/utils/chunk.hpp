/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 12:07:38 by etran             #+#    #+#             */
/*   Updated: 2023/07/06 12:09:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define BLOCK_SIZE 1
# define CHUNK_SIZE 16
# define CHUNK_HEIGHT 256

# define WORLD_WIDTH 1024 * CHUNK_SIZE	// 16384
# define WORLD_DEPTH WORLD_WIDTH		// 16384
# define WORLD_HEIGHT CHUNK_HEIGHT		// 256

# define DEFAULT_RENDER_DISTANCE 16