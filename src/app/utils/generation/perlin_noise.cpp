/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:26:08 by etran             #+#    #+#             */
/*   Updated: 2023/06/03 00:26:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_noise.hpp"
#include "math.hpp"
#include "vector.hpp"
#include "model.hpp"

#include <cstdint> // uint32_t
#include <vector> // std::vector
#include <algorithm> // std::shuffle

namespace vox {

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
PerlinNoise::PerlinNoise(
	PerlinNoise::NoiseMapInfo info
):
seed(info.seed.has_value() ? info.seed.value() : generateSeed()),
width(info.width),
height(info.height),
depth(info.depth),
layers(info.layers),
frequency(info.frequency_0),
frequency_mult(info.frequency_mult),
amplitude_mult(info.amplitude_mult),
generator(seed),
permutation_table(generatePermutationTable()) {
	std::vector<float>	(PerlinNoise::*generateNoiseMapFn)();

	// Select the correct function to generate the noise map
	if (info.type == PerlinNoiseType::PERLIN_NOISE_1D) {
		generateNoiseMapFn = &PerlinNoise::generate1dNoiseMap;
	} else if (info.type == PerlinNoiseType::PERLIN_NOISE_2D) {
		generateNoiseMapFn = &PerlinNoise::generate2dNoiseMap;
	} else if (info.type == PerlinNoiseType::PERLIN_NOISE_3D) {
		generateNoiseMapFn = &PerlinNoise::generate3dNoiseMap;
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

/**
 * @brief Converts the noise map to a model.
*/
PerlinNoise::PerlinMesh	PerlinNoise::toMesh() const {
	PerlinMesh			mesh;
	const float			half_width = width / 2;
	const float			half_height = height / 2;
	std::size_t			vertice_count = width * height;

	std::vector<scop::Vect3>	vertice_coords;
	std::vector<uint32_t>		indices;
	vertice_coords.reserve(vertice_count);
	indices.reserve(vertice_count);

	float scale = 64.0f / depth;
	float shift = 8.0f;

	// Store vertices coordinates
	for (std::size_t y = 0; y < height; ++y) {
		for (std::size_t x = 0; x < width; ++x) {
			std::size_t	i = std::fma(y, width, x); // y * width + x

			// Retrieve raw height from noise map
			vertice_coords.emplace_back(Vect3(
				x - half_width,							// x
				std::fma(noise_map[i], scale, -shift),	// y
				y - half_height							// z
			));

			// TODO: Missing indices, redo up to STOP
			// Upper left triangle: y * width + x
			indices.emplace_back(static_cast<uint32_t>(i));
			// Lower right triangle: (y + 1) * width + x
			indices.emplace_back(static_cast<uint32_t>(
				std::fma(y + 1, width, x)
			));

			// STOP
		}
	}
	mesh.vertices = std::move(vertice_coords);
	mesh.indices = std::move(indices);
	return mesh;
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

	for (std::size_t i = 0; i < table_sizes; ++i) {
		table[i] = generateFloat(0.0f, 1.0f);
	}
	return table;
}

/**
 * @brief Generate the gradient table.
*/
std::vector<scop::Vect3>	PerlinNoise::generateGradientTable() {
	std::vector<scop::Vect3>	table(table_sizes);

	for (std::size_t i = 0; i < table_sizes; ++i) {
		table[i] = scop::Vect3(
			generateFloat(-1.0f, 1.0f),
			generateFloat(-1.0f, 1.0f),
			generateFloat(-1.0f, 1.0f)
		);
		if (table[i] == scop::Vect3(0.0f, 0.0f, 0.0f)) {
			// If the gradient is null, regenerate it.
			--i;
			continue;
		}
		table[i] = scop::normalize(table[i]);
	}
	return table;
}

/**
 * @brief Generate the permutation table.
 *
 * @note This is basically a hash table.
*/
std::vector<std::size_t>	PerlinNoise::generatePermutationTable() {
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
float	PerlinNoise::evaluateAt(
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
std::size_t	PerlinNoise::hash(const float x) const noexcept {
	return permutation_table[static_cast<std::size_t>(x)];
}

/**
 * @brief Returns a value from the permutation table (2D noise).
*/
std::size_t	PerlinNoise::hash(
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
std::size_t	PerlinNoise::hash(
	const float x,
	const float y,
	const float z
) const noexcept {
	return permutation_table[
		permutation_table[
			permutation_table[static_cast<std::size_t>(x)] +
			static_cast<std::size_t>(y)
		] +
		static_cast<std::size_t>(z)
	];
}

/* ========================================================================== */

/**
 * @brief Generates a 1d noise map of size width.
 *
 * @note The noise map doesn't take in consideration the layers.
*/
std::vector<float>	PerlinNoise::generate1dNoiseMap() {
	std::vector<float>	noise_map(width);
	std::vector<float>	random_table = generateRandomTable();

	std::function<float(const float&)>	floorFn =
		[](const float& x){
		return std::floor(x);
	};

	std::function<float(const float&, int32_t)> modFn =
		[](const float& x, int32_t len){
		return static_cast<int32_t>(x) & len;
	};

	std::function<float(float, float, float)>	lerpFn =
		[&random_table, this](float x_min, float x_max, float t){
		return scop::math::lerp(
			random_table[hash(x_min)],
			random_table[hash(x_max)],
			t
		);
	};

	for (std::size_t x = 0; x < width; ++x) {
		noise_map[x] = evaluateAt(
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
std::vector<float>	PerlinNoise::generate2dNoiseMap() {
	std::vector<float>	noise_map(width * height);
	std::vector<float>	random_table = generateRandomTable();

	std::function<scop::Vect2(const scop::Vect2&)> floorFn =
		[](const scop::Vect2& vec){
		return scop::Vect2(std::floor(vec.x), std::floor(vec.y));
	};

	std::function<scop::Vect2(const scop::Vect2&, int32_t)> modFn =
		[](const scop::Vect2& vec, int32_t len){
		return scop::Vect2(
			static_cast<int32_t>(vec.x) & len,
			static_cast<int32_t>(vec.y) & len
		);
	};

	std::function<float(scop::Vect2, scop::Vect2, scop::Vect2)> lerpFn =
		[&random_table, this](scop::Vect2 min, scop::Vect2 max, scop::Vect2 t){
		// Retrieve corners.
		float c00 = random_table[hash(min.x, min.y)];
		float c10 = random_table[hash(max.x, min.y)];
		float c01 = random_table[hash(min.x, max.y)];
		float c11 = random_table[hash(max.x, max.y)];

		// Smoothen t.
		scop::Vect2 s = scop::Vect2(
			scop::math::smoothen(t.x),
			scop::math::smoothen(t.y)
		);

		// Interpolate between corners using s.
		return
			scop::math::lerp(
				scop::math::lerp(c00, c10, s.x),
				scop::math::lerp(c01, c11, s.x),
				s.y
			);
	};

	float	norm = 0;
	for (std::size_t y = 0; y < height; ++y) {
		for (std::size_t x = 0; x < width; ++x) {
			scop::Vect2	coord = scop::Vect2(x, y) * frequency;
			float		amplitude = 1;	// Amplitude of the layer.

			for (std::size_t layer = 0; layer < layers; ++layer) {
				// Evaluate and stack up layers.
				noise_map[y * width + x] = std::fma(
					evaluateAt(
						coord,
						floorFn,
						modFn,
						lerpFn,
						scop::Vect2(1.0f, 1.0f)
					),
					amplitude,
					noise_map[y * width + x]
				);
				coord *= frequency_mult;
				amplitude *= amplitude_mult;
			}

			// Retrieve max value for normalization.
			if (noise_map[y * width + x] > norm) {
				norm = noise_map[y * width + x];
			}
		}
	}
	for (auto& value: noise_map) {
		value /= norm;
	}
	return noise_map;
}

/**
 * @brief Generates a 3d noise map.
*/
std::vector<float>	PerlinNoise::generate3dNoiseMap() {
	std::vector<float>	noise_map(width * height * depth);
	std::vector<scop::Vect3>	gradients = generateGradientTable();

	std::function<scop::Vect3(const scop::Vect3&)>	floorFn =
		[](const scop::Vect3& vec){
		return scop::Vect3(
			std::floor(vec.x),
			std::floor(vec.y),
			std::floor(vec.z)
		);
	};

	std::function<scop::Vect3(const scop::Vect3&, int32_t)>	modFn =
		[](const scop::Vect3& vec, int32_t len){
		return scop::Vect3(
			static_cast<int32_t>(vec.x) & len,
			static_cast<int32_t>(vec.y) & len,
			static_cast<int32_t>(vec.z) & len
		);
	};

	std::function<float(scop::Vect3, scop::Vect3, scop::Vect3 t)>	lerpFn =
		[&gradients, this](scop::Vect3 min, scop::Vect3 max, scop::Vect3 t){
			// Retrieve gradients at corners.
			scop::Vect3 c000 = gradients[hash(min.x, min.y, min.z)];
			scop::Vect3 c100 = gradients[hash(max.x, min.y, min.z)];
			scop::Vect3 c010 = gradients[hash(min.x, max.y, min.z)];
			scop::Vect3 c110 = gradients[hash(max.x, max.y, min.z)];

			scop::Vect3 c001 = gradients[hash(min.x, min.y, max.z)];
			scop::Vect3 c101 = gradients[hash(max.x, min.y, max.z)];
			scop::Vect3 c011 = gradients[hash(min.x, max.y, max.z)];
			scop::Vect3 c111 = gradients[hash(max.x, max.y, max.z)];

			// Retrieve vectors from corners to point.
			scop::Vect3 p000 = scop::Vect3(t.x, t.y, t.z);
			scop::Vect3 p100 = scop::Vect3(t.x - 1, t.y, t.z);
			scop::Vect3 p010 = scop::Vect3(t.x, t.y - 1, t.z);
			scop::Vect3 p110 = scop::Vect3(t.x - 1, t.y - 1, t.z);

			scop::Vect3 p001 = scop::Vect3(t.x, t.y, t.z - 1);
			scop::Vect3 p101 = scop::Vect3(t.x - 1, t.y, t.z - 1);
			scop::Vect3 p011 = scop::Vect3(t.x, t.y - 1, t.z - 1);
			scop::Vect3 p111 = scop::Vect3(t.x - 1, t.y - 1, t.z - 1);

			// Smoothen t.
			scop::Vect3 s = scop::Vect3(
				scop::math::smoothen(t.x),
				scop::math::smoothen(t.y),
				scop::math::smoothen(t.z)
			);

			// Retrieve dot products.
			float x0 = scop::math::lerp(
				scop::dot(c000, p000),
				scop::dot(c100, p100),
				s.x
			);
			float x1 = scop::math::lerp(
				scop::dot(c010, p010),
				scop::dot(c110, p110),
				s.x
			);
			float x2 = scop::math::lerp(
				scop::dot(c001, p001),
				scop::dot(c101, p101),
				s.x
			);
			float x3 = scop::math::lerp(
				scop::dot(c011, p011),
				scop::dot(c111, p111),
				s.x
			);

			// Interpolate along y then z.
			return scop::math::lerp(
				scop::math::lerp(x0, x1, s.y),
				scop::math::lerp(x2, x3, s.y),
				s.z
			);
		};

	for (std::size_t z = 0; z < depth; ++z) {
		for (std::size_t y = 0; y < height; ++y) {
			for (std::size_t x = 0; x < width; ++x) {
				noise_map[z * width * height + y * width + x] = evaluateAt(
					scop::Vect3(x, y, z),
					floorFn,
					modFn,
					lerpFn,
					scop::Vect3(1.0f, 1.0f, 1.0f)
				);
			}
		}
	}
	return noise_map;
}

} // namespace scop

// TODO remove
// #include<iostream>
// #include <fstream>
// int main() {
// 	std::ofstream	file("noise_map.ppm", std::ios::out | std::ios::binary);

// 	scop::PerlinNoise::NoiseMapInfo	info = {
// 		.type = scop::PerlinNoiseType::PERLIN_NOISE_2D,
// 		.width = 450,
// 		.height = 450,
// 		.layers = 5,
// 		.frequency_0 = .02f,
// 		.frequency_mult = 1.8f,
// 		.amplitude_mult = 0.5f,
// 	};
// 	scop::PerlinNoise	noise(info);
// 	std::vector<float>	noise_map = noise.getNoiseMap();

// 	std::cout << "Seed: " << noise.getSeed() << std::endl;

// 	// Generate PPM file in format P6.
// 	file << "P6\n" << noise.getWidth() << " " << noise.getHeight() << "\n255\n";

// 	for (std::size_t y = 0; y < noise.getHeight(); ++y) {
// 		for (std::size_t x = 0; x < noise.getWidth(); ++x) {
// 			float	value = noise_map[y * noise.getWidth() + x];
// 			uint8_t	color = static_cast<uint8_t>(value * 255.0f);
// 			file << color << color << color;
// 		}
// 	}

// 	file.close();
// 	return 0;
// }