/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:30:06 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 15:02:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include <random>

namespace math {

f32 radians(f32 degrees) noexcept {
    return static_cast<f32>(degrees * M_PI / 180.0f);
}

f32 dregrees(f32 radians) noexcept {
    return static_cast<f32>(radians * 180.0f / M_PI);
}

f32 smoothen(const f32 x) noexcept {
    return std::fma(
        std::fma(std::fma(x, -2.0f, 3.0f), x, 0.0f),
        x,
        0.0f);
}

f32 generateRandomFloat() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<f32> dis(0.0f, 1.0f);
    return dis(gen);
}

} // namespace math