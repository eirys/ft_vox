/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_decl.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:35:46 by etran             #+#    #+#             */
/*   Updated: 2024/09/11 13:38:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_DECL_H
# define GAME_DECL_H

# define CHUNK_SIZE     16
# define CHUNK_HEIGHT   16 // Temporary value, will be changed to 256
# define CHUNK_AREA     (CHUNK_SIZE * CHUNK_SIZE) // 256
# define CHUNK_VOLUME   (CHUNK_AREA * CHUNK_HEIGHT) // 4096

# define NOISEMAP_SIZE  16
# define STAR_COUNT     (NOISEMAP_SIZE * NOISEMAP_SIZE)

# define WORLD_Y        { 0.0f, 1.0f, 0.0f }

# define WINDOW_WIDTH   1200.0f
# define WINDOW_HEIGHT  800.0f

# define CAM_FOV        85.0f

# define SSAO_KERNEL_SIZE   64
# define SSAO_RADIUS        0.5

#endif // GAME_DECL_H