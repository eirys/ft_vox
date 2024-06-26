/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vox_decl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 09:05:39 by etran             #+#    #+#             */
/*   Updated: 2024/06/25 16:02:29 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VOX_DECL_H
# define VOX_DECL_H

# define ENABLE_SKYBOX 1
# define ENABLE_CUBEMAP 0
# define ENABLE_FRUSTUM_CULLING 0
# define ENABLE_SHADOW_MAPPING 0
# define ENABLE_SSAO 0

#ifdef VOX_CPP

# define ENABLE_HIGH_RES 0

# if !ENABLE_SKYBOX && ENABLE_CUBEMAP
    static_assert(false, "Cubemap cannot be enabled if skybox is disabled");
# endif

# endif // VOX_CPP

#endif // VOX_DECL_H