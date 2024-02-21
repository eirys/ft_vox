/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 17:48:20 by etran             #+#    #+#             */
/*   Updated: 2024/02/21 11:22:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* ========================================================================== */
/*                               SYSTEM SPECIFIC                              */
/* ========================================================================== */

#ifdef __LINUX
# define __NL "\n"
#else
# define __NL "\r\n"
#endif // __LINUX


/* ========================================================================== */
/*                                   LOGGING                                  */
/* ========================================================================== */

#if defined(__DEBUG) || defined(__LOG) || defined(__INFO)
# include <ostream>
#endif

#ifndef __RELEASE
	#ifdef __DEBUG // For dev debug
	# define SCOP_DEBUG(X) std::cerr << "[[DEBUG]] " << X << __NL
	#else
	# define SCOP_DEBUG(X)
	#endif // __DEBUG
#endif // __RELEASE

#ifdef __INFO // For user/dev info
# define SCOP_INFO(X) std::cerr << "[[INFO]] " << X << __NL
#else
# define SCOP_INFO(X)
#endif // __INFO

#ifdef __LOG // For user log
#  define SCOP_LOG(X) std::cerr << "[[LOG]] " << X << __NL
#else
#  define SCOP_LOG(X)
#endif // __LOG

/* ========================================================================== */