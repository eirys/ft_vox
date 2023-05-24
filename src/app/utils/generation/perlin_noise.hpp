/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 18:33:56 by etran             #+#    #+#             */
/*   Updated: 2023/05/24 22:48:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector> // std::vector
# include <cstdint> // uint32_t
# include <functional> // std::function
# include <random> // std::mt19937

namespace scop {
class Vect2;
class Vect3;

enum PerlinNoiseType {
	PERLIN_NOISE_1D,
	PERLIN_NOISE_2D,
	PERLIN_NOISE_3D
};

/**
 * Perlin noise map generator.
*/
class PerlinNoise {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	PerlinNoise(std::size_t width, std::size_t height, uint32_t init_seed = 0);

	PerlinNoise(const PerlinNoise& other) = default;
	PerlinNoise(PerlinNoise&& other) = default;
	PerlinNoise& operator=(const PerlinNoise& other) = default;

	PerlinNoise() = delete;
	~PerlinNoise() = default;

	/* 1D NOISE MAP ============================================================ */

	std::vector<float>			generate1dNoiseMap();

	/* 2D NOISE MAP ============================================================ */

	std::vector<float>			generate2dNoiseMap();

	/* GETTERS ================================================================= */

	std::size_t					getWidth() const noexcept;
	std::size_t					getHeight() const noexcept;
	uint32_t					getSeed() const noexcept;

// private:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr const std::size_t	table_sizes = 256;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	const std::size_t			width;
	const std::size_t			height;
	const uint32_t				seed;
	std::mt19937				generator;

	std::vector<uint32_t>		permutation_table;
	std::vector<float>			random_table;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	uint32_t					generateSeed() const;
	float						generateFloat(float min, float max);
	std::vector<float>			generateRandomTable();
	std::vector<uint32_t>		generatePermutationTable();

	template <typename T>
	float						evaluateAt(
		const T& x,
		std::function<T(const T&)> floorFn,
		std::function<T(const T&, int32_t)> modFn,
		std::function<float(T, T, T)> lerpFn,
		T unit
	) const;

}; // class PerlinNoise

} // namespace scop