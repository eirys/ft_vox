/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_macros.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 10:42:08 by etran             #+#    #+#             */
/*   Updated: 2024/01/11 16:48:20 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MACROS_H
# define CHUNK_MACROS_H

# define CHUNK_SIZE					16 // Number of blocks per chunk row
# define CHUNK_AREA					(CHUNK_SIZE * CHUNK_SIZE)				// 256
# define CHUNK_VOLUME				(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)	// 4096

# define RENDER_DISTANCE			6  // Number of chunks to render
# define RENDER_DISTANCE2			(RENDER_DISTANCE * RENDER_DISTANCE)
# define RENDER_WIDTH				(RENDER_DISTANCE * CHUNK_SIZE)			// 256
# define RENDER_DEPTH				(RENDER_DISTANCE * CHUNK_SIZE)			// 256

# define MAX_RENDER_DISTANCE		32 // Local chunk + 31 chunks around it
# define RD_PYRAMID_BASE			(54 * 54) // 2916

# define MAX_RENDER_PYRAMID			(MAX_RENDER_DISTANCE * RD_PYRAMID_BASE / 3) // 31 104 chunks

# define FACE_COUNT					 6 // Number of faces per block


# define DISPATCH_SIZE 8
# define DISPATCH_TOTAL (DISPATCH_SIZE * DISPATCH_SIZE * DISPATCH_SIZE)


#endif