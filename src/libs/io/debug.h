/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 17:48:20 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 00:49:46 by etran            ###   ########.fr       */
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
# include <iostream>
#endif

#ifndef __RELEASE
	#ifdef __DEBUG // For dev debug
	# define LDEBUG(X) std::cerr << "[[DEBUG]] " << X << __NL
	#else
	# define LDEBUG(X)
	#endif // __DEBUG
#endif // __RELEASE

#ifdef __INFO // For user/dev info
# define LINFO(X) std::cerr << "[[INFO]] " << X << __NL
#else
# define LINFO(X)
#endif // __INFO

#ifdef __LOG // For user log
#  define LLOG(X) std::cerr << "[[LOG]] " << X << __NL
#else
#  define LLOG(X)
#endif // __LOG