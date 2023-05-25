/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:26:08 by etran             #+#    #+#             */
/*   Updated: 2023/05/25 10:54:23 by etran            ###   ########.fr       */
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
	PerlinNoise::NoiseMapInfo info
):	seed(info.seed.has_value() ? info.seed.value() : generateSeed()),
	width(info.width),
	height(info.height),
	layers(info.layers),
	frequency(info.frequency_0),
	frequency_mult(info.frequency_mult),
	amplitude_mult(info.amplitude_mult),
	generator(seed),
	permutation_table(generatePermutationTable()),
	random_table(generateRandomTable()) {
		if (info.type == PerlinNoiseType::PERLIN_NOISE_1D) {
			noise_map = generate1dNoiseMap();
		} else if (info.type == PerlinNoiseType::PERLIN_NOISE_2D) {
			noise_map = generate2dNoiseMap();
		// else if (info.type == PerlinNoiseType::PERLIN_NOISE_3D)
		// 	noise_map = generate3dNoiseMap();
		} else {
			throw std::invalid_argument("Invalid noise type.");
		}
	}

/* ========================================================================== */

std::vector<uint32_t>	PerlinNoise::toPixels() const {
	std::vector<uint32_t>	pixels(width * height);

	for (std::size_t i = 0; i < width * height; i++) {
		pixels[i] = static_cast<uint32_t>(noise_map[i] * 255);
	}
	return pixels;
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

const std::vector<float>&	PerlinNoise::getNoiseMap() const noexcept {
	return noise_map;
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

/* 1D NOISE MAP ============================================================= */

/**
 * @brief Generates a 1d noise map of size width.
*/
std::vector<float>	PerlinNoise::generate1dNoiseMap() {
	std::vector<float>	noise_map(width);

	std::function<float(float, float, float)>	lerpFn =
		[this](float x_min, float x_max, float t){
		return scop::math::lerp(
			random_table[static_cast<std::size_t>(x_min)],
			random_table[static_cast<std::size_t>(x_max)],
			t
		);
	};

	std::function<float(const float&)>	floorFn =
		[](const float& x){
		return std::floor(x);
	};

	std::function<float(const float&, int32_t)> modFn =
		[](const float& x, int32_t len){
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
		[this](scop::Vect2 min, scop::Vect2 max, scop::Vect2 t){
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

	std::function<scop::Vect2(const scop::Vect2&)> floorFn =
		[](const scop::Vect2& vec){
		return scop::Vect2(std::floor(vec.x),std::floor(vec.y));
	};

	std::function<scop::Vect2(const scop::Vect2&, int32_t)> modFn =
		[](const scop::Vect2& vec, int32_t len){
		return scop::Vect2(
			static_cast<int32_t>(vec.x) & len,
			static_cast<int32_t>(vec.y) & len
		);
	};

	const scop::Vect2	unit = scop::Vect2(1.0f, 1.0f);
	float	norm = 0;
	for (std::size_t y = 0; y < height; ++y) {
		for (std::size_t x = 0; x < width; ++x) {
			scop::Vect2	coord = scop::Vect2(x, y) * frequency;
			float		amplitude = 1;	// Amplitude of the layer.

			for (std::size_t layer = 0; layer < layers; ++layer) {
				// Evaluate and stack up layers.
				noise_map[y * width + x] += evaluateAt(
					coord * amplitude,
					floorFn,
					modFn,
					lerpFn,
					unit
				);
				// Increase noise frequency and decrease amplitude.
				coord *= frequency_mult;
				amplitude *= amplitude_mult;
			}

			// Retrieve max value for normalization.
			if (noise_map[y * width + x] > norm) {
				norm = noise_map[y * width + x];
			}
		}
	}
	if (norm == 0) {
		throw std::invalid_argument("Noise map normalization failed.");
	}
	for (auto& value: noise_map) {
		value /= norm;
	}
	return noise_map;
}

} // namespace scop

#include<iostream>
#include <fstream>
int main() {
	std::ofstream	file("noise_map.ppm", std::ios::out | std::ios::binary);

	scop::PerlinNoise::NoiseMapInfo	info = {
		.width = 450,
		.height = 450,
		.layers = 4,
		.frequency_0 = 0.04f,
		.frequency_mult = 1.5f,
		.amplitude_mult = 0.35f,
		.type = scop::PerlinNoiseType::PERLIN_NOISE_2D,
	};
	scop::PerlinNoise	noise(info);
	std::vector<float>	noise_map = noise.getNoiseMap();

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