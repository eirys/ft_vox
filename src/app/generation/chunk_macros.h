/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_macros.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 10:42:08 by etran             #+#    #+#             */
/*   Updated: 2024/02/04 12:18:34 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MACROS_H
# define CHUNK_MACROS_H

# define CHUNK_SIZE					16 // Number of blocks per chunk row
# define CHUNK_AREA					(CHUNK_SIZE * CHUNK_SIZE)				// 256
# define CHUNK_VOLUME				(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)	// 4096

# define RENDER_DISTANCE			6  // Number of chunks to render
# define RENDER_DISTANCE2			(RENDER_DISTANCE * RENDER_DISTANCE)
# define TOTAL_CHUNKS				(RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE)

# define MAX_RENDER_DISTANCE		32 // Local chunk + 31 chunks around it

# define FACE_COUNT					 6 // Number of faces per block

// Maximum faces per chunk
# define MAX_FACE_COUNT					\
	/* Each layer */					\
	(CHUNK_SIZE *						\
	(((FACE_COUNT - 1) +				\
	 ((FACE_COUNT - 2) * 2 * 15) +		\
	 ((FACE_COUNT - 3) * 15 * 15))) +	\
	/* Last face */						\
	CHUNK_AREA)
// In renderdoc, with no alignment specified, offsets are 13056/4 = 3264 bytes apart

# define DISPATCH_SIZE				(RENDER_DISTANCE / 2)
# define DISPATCH_TOTAL				(DISPATCH_SIZE * DISPATCH_SIZE * DISPATCH_SIZE)

# ifdef VOX_CPP
static_assert(DISPATCH_SIZE <= 8, "Dispatch size must be less than or equal to 8");
static_assert(RENDER_DISTANCE % 2 == 0, "Render distance must be a multiple of 2");
static_assert(RENDER_DISTANCE < MAX_RENDER_DISTANCE, "Render distance must not go beyond 32");
# endif // VOX_CPP

// Maximum vertices per chunk
# define VERTICES_TOTAL				(CHUNK_SIZE * FACE_COUNT)

#endif // CHUNK_MACROS_H