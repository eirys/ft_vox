/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constant_buffer_macros.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 10:41:07 by etran             #+#    #+#             */
/*   Updated: 2023/09/21 10:41:07 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define __ALIGNMENT_MAT4 16
# define __ALIGNMENT_VEC3 16
# define __ALIGNMENT_BUFF 64
# define __ALIGNMENT_SCAL(X) sizeof(X)