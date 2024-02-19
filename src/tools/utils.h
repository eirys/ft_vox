/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:16:25 by etran             #+#    #+#             */
/*   Updated: 2024/02/04 12:10:21 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define STR(X) #X

#include <initializer_list>

namespace scop::utils {

template <typename EnumClass>
constexpr inline uint32_t	enumSize() {
	return static_cast<uint32_t>(EnumClass::Last) - static_cast<uint32_t>(EnumClass::First) + 1;
}

template <typename EnumClass, typename Type = uint32_t>
constexpr inline EnumClass	enumToFlag(std::initializer_list<EnumClass> flags) {
	Type mask = 0;

	// for (EnumClass value = static_cast<EnumClass>(va_arg(args, Type)); value = static_cast<EnumClass>(va_arg(args, Type)))
	for (EnumClass value: flags)
		mask |= static_cast<Type>(value);
	return static_cast<EnumClass>(mask);
}

} // namespace scop::utils