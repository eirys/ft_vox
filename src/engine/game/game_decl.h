/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_decl.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:35:46 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 00:35:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_DECL_H
# define GAME_DECL_H

# define CHUNK_SIZE     16
# define CHUNK_HEIGHT   16 // Temporary value, will be changed to 256
# define CHUNK_AREA     (CHUNK_SIZE * CHUNK_SIZE) // 256
# define CHUNK_VOLUME   (CHUNK_AREA * CHUNK_HEIGHT) // 4096

# define WORLD_WIDTH    15
# define WORLD_DEPTH    WORLD_WIDTH
# define WORLD_SIZE     (WORLD_WIDTH * WORLD_DEPTH)

# define NOISEMAP_SIZE  16
# define STAR_COUNT     (NOISEMAP_SIZE * NOISEMAP_SIZE)

# define RENDER_DISTANCE 16 // Number of chunks around the player
# define RENDER_HEIGHT   1  // Number of chunks above and below the player
# define RENDER_AREA     (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_HEIGHT) // 2048

# define WORLD_ORIGIN   { RENDER_DISTANCE * CHUNK_SIZE * 0.5f, 0.0f, RENDER_DISTANCE * CHUNK_SIZE * 0.5f }

# define WORLD_Y        { 0.0f, 1.0f, 0.0f }


# define WINDOW_WIDTH   1200.0f
# define WINDOW_HEIGHT  800.0f

# define Z_NEAR         0.1f
# define Z_FAR          500.0f

# define CAM_FOV        85.0f


# define SSAO_KERNEL_SIZE   64
# define SSAO_RADIUS        0.5

#endif // GAME_DECL_H