/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_macros.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 10:42:08 by etran             #+#    #+#             */
/*   Updated: 2023/11/10 14:47:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MACROS_H
# define CHUNK_MACROS_H

# define CHUNK_SIZE					16 // Number of blocks per chunk row
# define RENDER_DISTANCE			5  // Number of chunks to render
# define RENDER_DISTANCE2			(RENDER_DISTANCE * RENDER_DISTANCE)
# define RENDER_WIDTH				(RENDER_DISTANCE * CHUNK_SIZE)			// 256
# define RENDER_DEPTH				(RENDER_DISTANCE * CHUNK_SIZE)			// 256
# define CHUNK_AREA					(CHUNK_SIZE * CHUNK_SIZE)				// 256
# define CHUNK_VOLUME				(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)	// 4096

# define BLOCK_FACES_COUNT			6

// 144 * uint32_t -> 288 * uint16_t -> Half of 576 chunks (max render distance: 24)
// At most 288 chunks can be rendered at once.
# define MAX_ACTIVE_CHUNKS			576
# define MAX_VISIBLE_CHUNKS			288 // 576 / 2

#endif