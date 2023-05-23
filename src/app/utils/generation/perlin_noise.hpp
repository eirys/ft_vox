/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 18:33:56 by etran             #+#    #+#             */
/*   Updated: 2023/05/23 18:09:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector> // std::vector
# include <cstdint> // uint32_t

namespace scop {
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

	PerlinNoise() = default;
	PerlinNoise(const PerlinNoise& other) = default;
	PerlinNoise(PerlinNoise&& other) = default;
	PerlinNoise& operator=(const PerlinNoise& other) = default;
	~PerlinNoise() = default;

	/* ========================================================================= */


private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	std::size_t					width;
	std::size_t					height;
	std::vector<scop::Vect3>	noise_map;
	const uin32_t				seed;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	uint32_t					generateSeed() const;
	float						generateFloat(
		uint32_t seed,
		float min,
		float max
	) const;
	std::vector<int>			generatePermutationTable(
		std::size_t len
	) const;

	/* 1D NOISE MAP ============================================================ */

	template <typename T>
	T							evaluateAt(
		const T& x, 
		T (*lerpFn)(T, T, float),
		T (*floorFn)(const T&),
		T (*fractFn)(const T&, const T&),
		T (*modFn)(const T&, const T&),
		const std::vector<T>& table
	) const;
	std::vector<float>			generate1dNoiseMap(std::size_t len) const;

	/* 2D NOISE MAP ============================================================ */

	std::vector<scop::Vect3>	generate2dNoiseMap() const;

}; // class PerlinNoise

} // namespace scop