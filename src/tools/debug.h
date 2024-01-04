/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 17:48:20 by etran             #+#    #+#             */
/*   Updated: 2024/01/02 18:20:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# ifdef __LINUX
# define __NL "\n"
# else
# define __NL "\r\n"
# endif

# ifdef __DEBUG
#  include <iostream>
#  define LOG(X) std::cerr << "[[LOG]] " << X << __NL
# else
#  define LOG(X)
# endif
