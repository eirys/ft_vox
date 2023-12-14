/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_macros.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 10:42:08 by etran             #+#    #+#             */
/*   Updated: 2023/12/11 20:54:43 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MACROS_H
# define CHUNK_MACROS_H

# define CHUNK_SIZE					16 // Number of blocks per chunk row
# define CHUNK_AREA					(CHUNK_SIZE * CHUNK_SIZE)				// 256
# define CHUNK_VOLUME				(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)	// 4096

# define RENDER_DISTANCE			5  // Number of chunks to render
# define RENDER_DISTANCE2			(RENDER_DISTANCE * RENDER_DISTANCE)
# define RENDER_WIDTH				(RENDER_DISTANCE * CHUNK_SIZE)			// 256
# define RENDER_DEPTH				(RENDER_DISTANCE * CHUNK_SIZE)			// 256

# define MAX_RENDER_DISTANCE		32 // Local chunk + 31 chunks around it

#endif