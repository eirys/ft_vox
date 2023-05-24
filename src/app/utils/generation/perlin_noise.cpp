/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:26:08 by etran             #+#    #+#             */
/*   Updated: 2023/05/24 23:43:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_noise.hpp"
#include "math.hpp"
#include "vector.hpp"

#include <cstdint> // uint32_t
#include <vector> // std::vector
#include <algorithm> // std::shuffle

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
	seed(init_seed ? init_seed : generateSeed()),
	generator(seed),
	permutation_table(generatePermutationTable()),
	random_table(generateRandomTable()) {}

/* 1D NOISE MAP ============================================================= */

/**
 * @brief Generates a 1d noise map of size width.
*/
std::vector<float>	PerlinNoise::generate1dNoiseMap() {
	std::vector<float>	noise_map(width);

	std::function<float(float, float, float)>	lerpFn = [this](
		float x_min,
		float x_max,
		float t
	){
		return scop::math::lerp(
			random_table[static_cast<std::size_t>(x_min)],
			random_table[static_cast<std::size_t>(x_max)],
			t
		);
	};

	std::function<float(const float&)>	floorFn = [](const float& x){
		return std::floor(x);
	};

	std::function<float(const float&, int32_t)> modFn = [](
		const float& x,
		int32_t len
	){
		return static_cast<int32_t>(x) & len;
	};

	for (std::size_t x = 0; x < width; ++x) {
		noise_map.emplace_back(evaluateAt(
			static_cast<float>(x),
			floorFn,
			modFn,
			lerpFn,
			1.0f
		));
	}
	return noise_map;
}

/* 2D NOISE MAP ============================================================= */

/**
 * @brief Generates a 2d noise map.
*/
std::vector<float>	PerlinNoise::generate2dNoiseMap() {
	std::vector<float>	noise_map(width * height);

	std::function<float(scop::Vect2, scop::Vect2, scop::Vect2)> lerpFn =
		[this](scop::Vect2 min,scop::Vect2 max,scop::Vect2 t){
		// Retrieve corners.
		float c00 = random_table[
			permutation_table[permutation_table[min.x] + min.y]
		];
		float c10 = random_table[
			permutation_table[permutation_table[max.x] + min.y]
		];
		float c01 = random_table[
			permutation_table[permutation_table[min.x] + max.y]
		];
		float c11 = random_table[
			permutation_table[permutation_table[max.x] + max.y]
		];

		// Smoothen
		scop::Vect2 s = scop::Vect2(
			scop::math::smoothen(t.x),
			scop::math::smoothen(t.y)
		);

		// Interpolate between corners.
		return
			scop::math::lerp(
				scop::math::lerp(c00, c10, s.x),
				scop::math::lerp(c01, c11, s.x),
				s.y
			);
	};

	std::function<scop::Vect2(const scop::Vect2&)> floorFn = [](
		const scop::Vect2& vec
	){
		return scop::Vect2(std::floor(vec.x),std::floor(vec.y));
	};

	std::function<scop::Vect2(const scop::Vect2&, int32_t)> modFn = [](
		const scop::Vect2& vec,
		int32_t len
	){
		return scop::Vect2(
			static_cast<int32_t>(vec.x) & len,
			static_cast<int32_t>(vec.y) & len
		);
	};

	for (std::size_t y = 0; y < height; ++y) {
		for (std::size_t x = 0; x < width; ++x) {
			noise_map[y * width + x] = evaluateAt(
				scop::Vect2(x, y) * 0.5f,
				floorFn,
				modFn,
				lerpFn,
				scop::Vect2(1.0f, 1.0f)
			);
		}
	}
	return noise_map;
}

/* GETTERS ================================================================== */

std::size_t	PerlinNoise::getWidth() const noexcept {
	return width;
}

std::size_t	PerlinNoise::getHeight() const noexcept {
	return height;
}

uint32_t	PerlinNoise::getSeed() const noexcept {
	return seed;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Generate a random seed when no seed is provided.
*/
uint32_t	PerlinNoise::generateSeed() const {
	return static_cast<uint32_t>(std::random_device()());
}

/**
 * @brief Generate a random float between min and max, using seed.
*/
float	PerlinNoise::generateFloat(float min, float max) {
	std::uniform_real_distribution<float> dis(min, max);
	return dis(generator);
}

/**
 * @brief Generate the random table.
*/
std::vector<float>	PerlinNoise::generateRandomTable() {
	std::vector<float>	table(table_sizes);

	for (std::size_t i = 0; i < table_sizes; i++) {
		table[i] = generateFloat(0.0f, 1.0f);
	}
	return table;
}

/**
 * @brief Generate the permutation table.
*/
std::vector<uint32_t>	PerlinNoise::generatePermutationTable() {
	std::size_t				size = table_sizes * 2;
	std::vector<uint32_t>	table(size);

	// Fill table.
	for (std::size_t i = 0; i < table_sizes; ++i) {
		table[i] = static_cast<uint32_t>(i);
	}
	// Shuffle table using the seed.
	std::shuffle(table.begin(), table.begin() + table_sizes, generator);

	// Duplicate the table.
	for (std::size_t i = 0; i < table_sizes; ++i) {
		table[table_sizes + i] = table[i];
	}
	return table;
}

/* ========================================================================== */

/**
 * @brief Returns an interpolated value from the permutation table.
 *
 * @param x			The coordinate to evaluate at.
 * @param floorFn	Floor function.
 * @param modFn		Modulo function.
 * @param lerpFn	Linear interpolation function.
 * @param unit		Corresponds a coordinate unit.
 *
 * @note			- T is the type of the coordinate.
*/
template <typename T>
float	PerlinNoise::evaluateAt(
	const T& x,
	std::function<T(const T&)> floorFn,
	std::function<T(const T&, int32_t)> modFn,
	std::function<float(T, T, T)> lerpFn,
	T unit
) const {
	// Get the integer part of x.
	T	x_int = floorFn(x);
	// Get the fractional part of x.
	T	t = x - x_int;

	// Retrieve upper and lower bounds.
	T	x_min = modFn(x_int, table_sizes - 1);
	T	x_max = modFn(x_min + unit, table_sizes - 1);
	return lerpFn(x_min, x_max, t);
}

} // namespace scop

#include<iostream>
#include <fstream>
int main() {
	std::ofstream	file("noise_map.ppm", std::ios::out | std::ios::binary);

	scop::PerlinNoise	noise(256, 256);
	std::vector<float>	noise_map = noise.generate2dNoiseMap();

	std::cout << "Seed: " << noise.getSeed() << std::endl;

	// Generate PPM file in format P6.
	file << "P6\n" << noise.getWidth() << " " << noise.getHeight() << "\n255\n";

	for (std::size_t y = 0; y < noise.getHeight(); ++y) {
		for (std::size_t x = 0; x < noise.getWidth(); ++x) {
			float	value = noise_map[y * noise.getWidth() + x];
			uint8_t	color = static_cast<uint8_t>(value * 255.0f);
			file << color << color << color;
		}

	}

	file.close();
	return 0;
}