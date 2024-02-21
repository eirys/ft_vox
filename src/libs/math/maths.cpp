/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:30:06 by etran             #+#    #+#             */
/*   Updated: 2024/02/21 11:37:55 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include <random>

namespace math {

constexpr f32   radians(f32 degrees) noexcept {
    return static_cast<f32>(degrees * M_PI / 180);
}

constexpr f32 dregrees(f32 radians) noexcept {
    return static_cast<f32>(radians * 180 / M_PI);
}

f32           smoothen(const f32 x) noexcept {
    return std::fma(
    std::fma(std::fma(x, -2.0f, 3.0f), x, 0.0f),
    x,
    0.0f);
}

f32           generateRandomFloat() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<f32> dis(0.0f, 1.0f);
    return dis(gen);
}

void            generateVibrantColor(f32& red, f32& green, f32& blue) {
    red = generateRandomFloat();
    green = generateRandomFloat();
    blue = generateRandomFloat();

    // Boost the saturation and value to make it vibrant
    const f32 maxChannel = std::max(std::max(red, green), blue);
    const f32 minChannel = std::min(std::min(red, green), blue);
    const f32 delta = maxChannel - minChannel;

    if (delta > 0.0f) {
        if (maxChannel == red) {
            green = (green - minChannel) / delta;
            blue = (blue - minChannel) / delta;
        }
        else if (maxChannel == green) {
            red = (red - minChannel) / delta;
            blue = (blue - minChannel) / delta;
        }
        else if (maxChannel == blue) {
            red = (red - minChannel) / delta;
            green = (green - minChannel) / delta;
        }
    }
}

} // namespace math