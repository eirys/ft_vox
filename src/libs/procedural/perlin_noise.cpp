/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:26:08 by etran             #+#    #+#             */
/*   Updated: 2024/08/26 13:24:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_noise.h"
#include "maths.h"
#include "vector.h"

#include <fstream>
// #include "block.h"

#include <cassert> // assert
#include <vector> // std::vector
#include <algorithm> // std::shuffle

namespace proc {

using vec2 = math::vec2;
using vec3 = math::vec3;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Perlin noise generator.
 *
 * @param info	The information needed to create the noise map.
 * 				See `PerlinNoise::NoiseMapInfo` for more information.
 *
 * @note No verification is done on the parameters.
*/
PerlinNoise::PerlinNoise(const NoiseMapInfo& info):
	seed(info.seed.has_value() ? info.seed.value() : _generateSeed()),
	width(info.width),
	height(info.height),
	depth(info.depth),
	layers(info.layers),
	frequency(info.frequency_0),
	frequency_mult(info.frequency_mult),
	amplitude_mult(info.amplitude_mult),
    scale(info.scale),
    shift(info.shift),
	generator(seed),
	permutation_table(_generatePermutationTable())
{
	std::vector<float>	(PerlinNoise::*generateNoiseMapFn)();

	// Select the correct function to generate the noise map
	if (info.type == PerlinNoiseType::PERLIN_NOISE_1D) {
		generateNoiseMapFn = &PerlinNoise::_generate1dNoiseMap;
	} else if (info.type == PerlinNoiseType::PERLIN_NOISE_2D) {
		generateNoiseMapFn = &PerlinNoise::_generate2dNoiseMap;
	} else if (info.type == PerlinNoiseType::PERLIN_NOISE_3D) {
		generateNoiseMapFn = &PerlinNoise::_generate3dNoiseMap;
	} else {
		throw std::invalid_argument("Invalid noise type.");
	}

	noise_map = (this->*generateNoiseMapFn)();
}

/* ========================================================================== */

/**
 * @brief Converts the noise map to a vector of pixels. (image)
*/
std::vector<uint32_t>	PerlinNoise::toPixels() const {
	std::vector<uint32_t>	pixels(width * height);

	for (std::size_t i = 0; i < width * height; ++i) {
		// Convert to grayscale
		pixels[i] = static_cast<uint32_t>(noise_map[i] * 255);
	}
	return pixels;
}

void PerlinNoise::copyToBuffer(u32* buffer) const {
    for (u32 i = 0; i < width * height; ++i) {
        const u8 val = (u8)(noise_map[i] * 255.0);
        buffer[i] = val;
    }
}

/**
 * @brief Save the noise map to a ppm file.
*/
void PerlinNoise::debug(const std::string& outfile) const {
    std::ofstream file(outfile);
    file << "P6\n" << width << " " << height << "\n255\n";
    for (std::size_t i = 0; i < width * height; ++i) {
        file << (char)(noise_map[i] * 255);
        file << (char)(noise_map[i] * 255);
        file << (char)(noise_map[i] * 255);
    }
}

float	PerlinNoise::noiseAt(std::size_t x) const noexcept {
	assert(x < width);

	float noise = noise_map[width + x];
	return std::floor(std::fma(noise, scale, shift));
}

float	PerlinNoise::noiseAt(std::size_t x, std::size_t y) const noexcept {
	assert(x < width);
	assert(y < height);

	float noise = noise_map[std::fma(y, width, x)]; // 0 to 1
	return std::floor(std::fma(noise, scale, shift));
}

float	PerlinNoise::noiseAt(
	std::size_t x,
	std::size_t y,
	std::size_t z
) const noexcept {
	assert(x < width);
	assert(y < height);
	assert(z < depth);

	float noise = noise_map[std::fma(std::fma(z, height, y), width, x)];
	return std::floor(std::fma(noise, scale, shift));
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
uint32_t	PerlinNoise::_generateSeed() const {
	return static_cast<uint32_t>(std::random_device()());
}

/**
 * @brief Generate a random float between min and max, using seed.
*/
float	PerlinNoise::_generateFloat(float min, float max) {
	std::uniform_real_distribution<float> dis(min, max);
	return dis(generator);
}

/**
 * @brief Generate the random table.
*/
std::vector<float>	PerlinNoise::_generateRandomTable() {
	std::vector<float>	table(table_sizes);

	for (std::size_t i = 0; i < table_sizes; ++i) {
		table[i] = _generateFloat(0.0f, 1.0f);
	}
	return table;
}

/**
 * @brief Generate the gradient table.
*/
std::vector<vec3>	PerlinNoise::_generateGradientTable() {
	std::vector<vec3>	table(table_sizes);

	for (std::size_t i = 0; i < table_sizes; ++i) {
		table[i] = vec3(
			_generateFloat(-1.0f, 1.0f),
			_generateFloat(-1.0f, 1.0f),
			_generateFloat(-1.0f, 1.0f)
		);
		if (table[i] == vec3(0.0f, 0.0f, 0.0f)) {
			// If the gradient is null, regenerate it.
			--i;
			continue;
		}
		table[i] = normalize(table[i]);
	}
	return table;
}

/**
 * @brief Generate the permutation table.
 *
 * @note This is basically a hash table.
*/
std::vector<std::size_t>	PerlinNoise::_generatePermutationTable() {
	std::size_t					size = table_sizes * 2;
	std::vector<std::size_t>	table(size);

	// Fill table.
	for (std::size_t i = 0; i < table_sizes; ++i) {
		table[i] = i;
	}
	// Shuffle table using the seed.
	std::shuffle(table.begin(), table.begin() + table_sizes, generator);

	// Duplicate the table.
	for (std::size_t i = 0; i < table_sizes; ++i) {
		table[table_sizes + i] = table[i];
	}
	return table;
}

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
float	PerlinNoise::_evaluateAt(
	const T& x,
	std::function<T(const T&)> floorFn,
	std::function<T(const T&, int32_t)> modFn,
	std::function<float(T, T, T)> lerpFn,
	const T unit
) const {
	// Get the integer part of x.
	const T	x_int = floorFn(x);
	// Get the fractional part of x.
	const T	t = x - x_int;

	// Retrieve upper and lower bounds.
	const T	x_min = modFn(x_int, table_sizes - 1);
	const T	x_max = modFn(x_min + unit, table_sizes - 1);
	return lerpFn(x_min, x_max, t);
}

/**
 * @brief Returns a value from the permutation table (1D noise).
*/
std::size_t	PerlinNoise::_hash(const float x) const noexcept {
	return permutation_table[static_cast<std::size_t>(x)];
}

/**
 * @brief Returns a value from the permutation table (2D noise).
*/
std::size_t	PerlinNoise::_hash(
	const float x,
	const float y
) const noexcept {
	return permutation_table[
		permutation_table[static_cast<std::size_t>(x)] +
		static_cast<std::size_t>(y)
	];
}

/**
 * @brief Returns a value from the permutation table (3D noise).
*/
std::size_t	PerlinNoise::_hash(
	const float x,
	const float y,
	const float z
) const noexcept {
	return permutation_table[
		permutation_table[
			permutation_table[static_cast<std::size_t>(x)] +
			static_cast<std::size_t>(y)
		] + static_cast<std::size_t>(z)
	];
}

/* ========================================================================== */

/**
 * @brief Generates a 1d noise map of size width.
 *
 * @note The noise map doesn't take in consideration the layers.
*/
std::vector<float>	PerlinNoise::_generate1dNoiseMap() {
	std::vector<float>	noise_map(width);
	std::vector<float>	random_table = _generateRandomTable();

	std::function<float(const float&)>	floorFn =
		[]
		(const float& x) -> float {
			return std::floor(x);
		};

	std::function<float(const float&, int32_t)> modFn =
		[]
		(const float& x, int32_t len) -> float {
			return static_cast<int32_t>(x) & len;
		};

	std::function<float(float, float, float)>	lerpFn =
		[&random_table, this]
		(float x_min, float x_max, float t) -> float {
			return math::lerp(
				random_table[_hash(x_min)],
				random_table[_hash(x_max)],
				t
			);
		};

	for (std::size_t x = 0; x < width; ++x) {
		noise_map[x] = _evaluateAt(
			static_cast<float>(x),
			floorFn,
			modFn,
			lerpFn,
			1.0f
		);
	}
	return noise_map;
}

/**
 * @brief Generates a 2d noise map.
*/
std::vector<float>	PerlinNoise::_generate2dNoiseMap() {
	std::vector<float>	noise_map(width * height);
	std::vector<float>	random_table = _generateRandomTable();

	std::function<vec2(const vec2&)> floorFn =
		[]
		(const vec2& vec) -> vec2 {
			return vec2(std::floor(vec.x), std::floor(vec.y));
		};

	std::function<vec2(const vec2&, int32_t)> modFn =
		[]
		(const vec2& vec, int32_t len) -> vec2 {
			return vec2(
				static_cast<int32_t>(vec.x) & len,
				static_cast<int32_t>(vec.y) & len
			);
		};

	std::function<float(vec2, vec2, vec2)> lerpFn =
		[&random_table, this]
		(vec2 min, vec2 max, vec2 t) -> float {
			// Retrieve corners.
			float c00 = random_table[_hash(min.x, min.y)];
			float c10 = random_table[_hash(max.x, min.y)];
			float c01 = random_table[_hash(min.x, max.y)];
			float c11 = random_table[_hash(max.x, max.y)];

			// Smoothen t.
			vec2 s = vec2(
				math::smoothen(t.x),
				math::smoothen(t.y));

			// Interpolate between corners using s.
			return math::lerp(
				math::lerp(c00, c10, s.x),
				math::lerp(c01, c11, s.x),
				s.y);
		};

	float	norm = 0;
	for (std::size_t y = 0; y < height; ++y) {
		for (std::size_t x = 0; x < width; ++x) {
			vec2	coord = vec2(x, y) * frequency;
			float		amplitude = 1;	// Amplitude of the layer.

			// Evaluate and stack up layers.
			for (std::size_t layer = 0; layer < layers; ++layer) {
				noise_map[y * width + x] =
					std::fma(
						_evaluateAt(
							coord,
							floorFn,
							modFn,
							lerpFn,
							vec2(1.0f, 1.0f)),
						amplitude,
						noise_map[y * width + x]);

				coord *= frequency_mult;
				amplitude *= amplitude_mult;
			}

			// Retrieve max value for normalization.
			if (noise_map[y * width + x] > norm) {
				norm = noise_map[y * width + x];
			}
		}
	}

	// Normalize noise map to [0, 1]: divide by norm.
	for (auto& value: noise_map) {
		value /= norm;
	}
	return noise_map;
}

/**
 * @brief Generates a 3d noise map.
*/
std::vector<float>	PerlinNoise::_generate3dNoiseMap() {
	std::vector<float>	noise_map(width * height * depth);
	std::vector<vec3>	gradients = _generateGradientTable();

	std::function<vec3(const vec3&)>	floorFn =
		[]
		(const vec3& vec) -> vec3 {
			return vec3(
				std::floor(vec.x),
				std::floor(vec.y),
				std::floor(vec.z)
			);
		};

	std::function<vec3(const vec3&, int32_t)>	modFn =
		[]
		(const vec3& vec, int32_t len) -> vec3 {
			return vec3(
				static_cast<int32_t>(vec.x) & len,
				static_cast<int32_t>(vec.y) & len,
				static_cast<int32_t>(vec.z) & len
			);
		};

	std::function<float(vec3, vec3, vec3 t)>	lerpFn =
		[&gradients, this]
		(vec3 min, vec3 max, vec3 t) -> float {
			// Retrieve gradients at corners.
			vec3 c000 = gradients[_hash(min.x, min.y, min.z)];
			vec3 c100 = gradients[_hash(max.x, min.y, min.z)];
			vec3 c010 = gradients[_hash(min.x, max.y, min.z)];
			vec3 c110 = gradients[_hash(max.x, max.y, min.z)];

			vec3 c001 = gradients[_hash(min.x, min.y, max.z)];
			vec3 c101 = gradients[_hash(max.x, min.y, max.z)];
			vec3 c011 = gradients[_hash(min.x, max.y, max.z)];
			vec3 c111 = gradients[_hash(max.x, max.y, max.z)];

			// Retrieve vectors from corners to point.
			vec3 p000 = vec3(t.x, t.y, t.z);
			vec3 p100 = vec3(t.x - 1, t.y, t.z);
			vec3 p010 = vec3(t.x, t.y - 1, t.z);
			vec3 p110 = vec3(t.x - 1, t.y - 1, t.z);

			vec3 p001 = vec3(t.x, t.y, t.z - 1);
			vec3 p101 = vec3(t.x - 1, t.y, t.z - 1);
			vec3 p011 = vec3(t.x, t.y - 1, t.z - 1);
			vec3 p111 = vec3(t.x - 1, t.y - 1, t.z - 1);

			// Smoothen t.
			vec3 s = vec3(
				math::smoothen(t.x),
				math::smoothen(t.y),
				math::smoothen(t.z)
			);

			// Retrieve dot products.
			float x0 = math::lerp(
				dot(c000, p000),
				dot(c100, p100),
				s.x
			);
			float x1 = math::lerp(
				dot(c010, p010),
				dot(c110, p110),
				s.x
			);
			float x2 = math::lerp(
				dot(c001, p001),
				dot(c101, p101),
				s.x
			);
			float x3 = math::lerp(
				dot(c011, p011),
				dot(c111, p111),
				s.x
			);

			// Interpolate along y then z.
			return math::lerp(
				math::lerp(x0, x1, s.y),
				math::lerp(x2, x3, s.y),
				s.z
			);
		};

	for (std::size_t z = 0; z < depth; ++z) {
		for (std::size_t y = 0; y < height; ++y) {
			for (std::size_t x = 0; x < width; ++x) {
				std::size_t	index = std::fma(
					z * width,
					height,
					std::fma(y, width, x)
				);
				noise_map[index] = _evaluateAt(
					vec3(x, y, z),
					floorFn,
					modFn,
					lerpFn,
					vec3(1.0f, 1.0f, 1.0f)
				);
			}
		}
	}
	return noise_map;
}

} // namespace proc