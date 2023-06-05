/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:16:47 by etran             #+#    #+#             */
/*   Updated: 2023/06/05 09:31:31 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <cmath> // M_PI, std::fma
# include <algorithm> // std::max, std::min
# include <random> // std::mt19937, std::uniform_real_distribution

namespace scop {
namespace math {
	
/**
 * @brief Converts degrees to radians.
*/
inline constexpr float	radians(float degrees) noexcept {
	return degrees * M_PI / 180;
}

/**
 * @brief Converts radians to degrees.
*/
inline constexpr float	dregrees(float radians) noexcept {
	return radians * 180 / M_PI;
}

/**
 * @brief Smoothen a value between 0.0f and 1.0f.
*/
inline constexpr float	smoothen(const float x) noexcept {
	return std::fma(
		std::fma(
			std::fma(x, -2.0f, 3.0f),
			x,
			0.0f
		),
		x,
		0.0f
	);
}

/**
 * @brief Generates a random float between 0.0f and 1.0f.
 * 
 * @note Uses a Mersenne Twister pseudo-random generator.
*/
inline float	generateRandomFloat() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	return dis(gen);
}

/**
 * @brief Generates a random vibrant color.
*/
inline void	generateVibrantColor(float& red, float& green, float& blue) {
	red = generateRandomFloat();
	green = generateRandomFloat();
	blue = generateRandomFloat();

	// Boost the saturation and value to make it vibrant
	const float maxChannel = std::max(std::max(red, green), blue);
	const float minChannel = std::min(std::min(red, green), blue);
	const float delta = maxChannel - minChannel;
	
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

/**
 * @brief Computes the linear interpolation between two values.
 *
 * @param x,y	The values to interpolate between.
 * @param t		The interpolation factor.
 * 
 * @note T must be a basic type (float, int, etc.).
*/
template <typename T>
inline float	lerp(T x, T y, float t) {
	return std::fma(t, y - x, x);
}

/**
 * @brief Special fma for Vect3 and float: a * b + c.
 * 
 * @param a		The vector to multiply.
 * @param b		The scalar to multiply.
 * @param c		The vector to add.
*/
inline Vect3	fma(const Vect3& a, float b, const Vect3& c) {
	return Vect3(
		std::fma(a.x, b, c.x),
		std::fma(a.y, b, c.y),
		std::fma(a.z, b, c.z)
	);
}

} // namespace math
} // namespace scop