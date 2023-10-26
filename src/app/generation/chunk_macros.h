/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_macros.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 10:42:08 by etran             #+#    #+#             */
/*   Updated: 2023/10/05 19:10:18 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MACROS_H
# define CHUNK_MACROS_H

# define CHUNK_SIZE					16 // Number of blocks per chunk row
# define RENDER_DISTANCE			5  // Number of chunks to render
# define RENDER_WIDTH				(RENDER_DISTANCE * CHUNK_SIZE)			// 256
# define RENDER_DEPTH				(RENDER_DISTANCE * CHUNK_SIZE)			// 256
# define CHUNK_AREA					(CHUNK_SIZE * CHUNK_SIZE)				// 256
# define CHUNK_VOLUME				(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)	// 4096

# define BLOCK_FACES_COUNT			6

// Number of vertices in a flat chunk mesh
# define BLOCK_VERTICES_COUNT		BLOCK_FACES_COUNT * 4					// 24
# define CHUNK_VERTICES_COUNT		CHUNK_AREA * BLOCK_VERTICES_COUNT

// Number of indices in a flat chunk mesh
# define TRIANGLE_VERTICES_COUNT	3
# define QUAD_VERTICES_COUNT		(2 * TRIANGLE_VERTICES_COUNT)
# define CHUNK_INDICES_COUNT		(CHUNK_AREA * BLOCK_FACES_COUNT * QUAD_VERTICES_COUNT)

#endif