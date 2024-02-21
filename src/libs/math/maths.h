/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:25:05 by etran             #+#    #+#             */
/*   Updated: 2024/02/21 11:38:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <cmath>
# include "types.h"

namespace math {

constexpr f32   radians(f32 degrees) noexcept;
constexpr f32   dregrees(f32 radians) noexcept;
f32             smoothen(const f32 x) noexcept;
f32             generateRandomf32();
void            generateVibrantColor(f32& red, f32& green, f32& blue);

template <typename T>
f32           lerp(T x, T y, f32 t) noexcept {
	return std::fma(t, y - x, x);
}

} // namespace math