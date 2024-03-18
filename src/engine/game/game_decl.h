/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_decl.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:35:46 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 16:13:17 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_DECL_H
# define GAME_DECL_H

# define CHUNK_SIZE     16
# define CHUNK_HEIGHT   16 // Temporary value, will be changed to 256
# define CHUNK_AREA     (CHUNK_SIZE * CHUNK_SIZE) // 256
# define CHUNK_VOLUME   (CHUNK_AREA * CHUNK_HEIGHT) // 4096

# define WORLD_WIDTH    5
# define WORLD_DEPTH    5
# define WORLD_SIZE     (WORLD_WIDTH * WORLD_DEPTH)

#endif // GAME_DECL_H