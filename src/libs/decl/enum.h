/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:42:03 by etran             #+#    #+#             */
/*   Updated: 2024/02/21 11:42:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "types.h"

template <typename EnumClass>
constexpr inline u32	enumSize() {
	return static_cast<u32>(EnumClass::Last) - static_cast<u32>(EnumClass::First) + 1;
}