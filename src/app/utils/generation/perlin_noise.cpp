/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:26:08 by etran             #+#    #+#             */
/*   Updated: 2023/05/23 18:11:30 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_noise.hpp"
#include "math.hpp"

#include <random> // std::mt19937

namespace scop {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Perlin noise generator, with no seed.
*/
PerlinNoise::PerlinNoise(
	std::size_t width,
	std::size_t height,
	uint32_t init_seed
):	width(width),
	height(height),
	noise_map(width * height),
	noise_map_1d(generate1dNoiseMap()),
	seed(init_seed ? init_seed : generateSeed()) {}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Generate a random float between min and max, using seed.
*/
float	PerlinNoise::generateFloat(uint32_t seed, float min, float max) const {
	std::mt19937 gen(seed);
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

/**
 * @brief Generate a random seed when no seed is provided.
*/
uint32_t	PerlinNoise::generateSeed() const {
	return static_cast<uint32_t>(std::random_device()());
}

/**
 * @brief Generate a permutation table.
 *
 * @param len The length of the table.
*/
std::vector<float>	PerlinNoise::generatePermutationTable(
	std::size_t len
) const {
	std::vector<float>	table(len);

	for (std::size_t i = 0; i < len; i++) {
		table.emplace_back(generateFloat(seed, 0.0f, 1.0f));
	}
	return table;
}

/**
 * @brief Returns an interpolated value from the permutation table.
 * 
 * @param x			The coordinate to evaluate at.
 * @param lerpFn	Linear interpolation function.
 * @param floorFn	Floor function.
 * @param fractFn	Fractional part function.
 * @param modFn		Modulo function.
 * @param table		The permutation table.
 *
 * @note			- T is the type of the coordinate.
*/
template <typename T>
T	PerlinNoise::evaluateAt(
	const T& x, 
	T (*lerpFn)(T, T, float),
	T (*floorFn)(const T&),
	T (*fractFn)(const T&, const T&),
	T (*modFn)(const T&, const T&),
	const std::vector<T>& table
) const {
	// Get the integer part of x.
	T	x_int = floorFn(x);
	// (
	// 	static_cast<int32_t>(x) -
	// 	(x < 0 && x != static_cast<int32_t>(x))
	// );

	// Get the fractional part of x.
	T	t = fractFn(x);
	// x - x_int;

	// Retrieve upper and lower bounds.
	// Using & instead of % (faster than checking if x_int is negative).
	T	x_min = modFn(x_int, table.size() - 1);
	//x_int & (table.size() - 1);
	T	x_max = modFn(x_min + 1, table.size() - 1);
	//(x_min + 1) & (table.size() - 1);
	// return scop::math::lerp(table[x_min], table[x_max], t);
	return lerpFn(table[x_min], table[x_max], t);
}

/* 1D NOISE MAP ============================================================= */

/**
 * @brief Generate a 1d noise map.
*/
std::vector<float>	PerlinNoise::generate1dNoiseMap(std::size_t len) const {
	std::vector<float>	noise_map(len);
	std::vector<float>	permutation_table = generatePermutationTable(len / 2);

	auto lerpFn = &scop::math::lerp;
	auto floorFn = [](const float& x){
		return static_cast<int32_t>(x) - (x < 0 && x != static_cast<int32_t>(x));
	};
	auto fractFn = [](const float& x, const float& x_int){
		return x - x_int;
	};
	auto modFn = [](const float& x, const float& len){
		return static_cast<int32_t>(x) & (len - 1);
	};

	for (std::size_t x = 0; x < len; ++x) {
		noise_map.emplace_back(evaluateAt(
			x,
			&scop::math::lerp,
			&floorFn,
			&fractFn,
			&modFn,
			permutation_table
		));
	}
	return noise_map;
}

/* 2D NOISE MAP ============================================================= */



} // namespace scop