/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:16:25 by etran             #+#    #+#             */
/*   Updated: 2024/02/21 11:21:43 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "types.h"
# define STR(X) #X

#include <initializer_list>

namespace scop::utils {

template <typename EnumClass>
constexpr inline u32	enumSize() {
	return static_cast<u32>(EnumClass::Last) - static_cast<u32>(EnumClass::First) + 1;
}

template <typename EnumClass, typename Type = u32>
constexpr inline EnumClass	enumToFlag(std::initializer_list<EnumClass> flags) {
	Type mask = 0;

	// for (EnumClass value = static_cast<EnumClass>(va_arg(args, Type)); value = static_cast<EnumClass>(va_arg(args, Type)))
	for (EnumClass value: flags)
		mask |= static_cast<Type>(value);
	return static_cast<EnumClass>(mask);
}

} // namespace scop::utils