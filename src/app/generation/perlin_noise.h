/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:13:52 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 23:10:15 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector> // std::vector
# include <cstdint> // uint32_t
# include <functional> // std::function
# include <random> // std::mt19937
# include <optional> // std::optional

# include "vector.h"
# include "vertex.h"

namespace vox {

typedef struct scop::Vect2	Vect2;
typedef	struct scop::Vect3	Vect3;

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
	/*                                HELPER CLASS                               */
	/* ========================================================================= */

	/**
	 * @brief Contains the information needed to create a noise map.
	 *
	 * @param type				The type of noise map to generate.
	 * @param seed				The seed to use for the noise map.
	 * 							If not provided, a random seed is generated.
	 * @param width				The width of the noise map. (1D/2D/3D noise)
	 * @param height			The height of the noise map. (2D/3D noise)
	 * @param depth				The depth of the noise map. (3D noise)
	 * @param layers			The number of layers to use for the noise map.
	 * 							More layers means more details.
	 * @param frequency_0		The frequency of the first layer.
	 * 							A higher frequency means slopes are steeper.
	 * @param frequency_mult	The frequency multiplier to use for each layer.
	 * 							A higher value means a rockier noise map.
	 * @param amplitude_mult	The amplitude multiplier to use for each layer.
	 * 							A higher value means a rockier noise map.
	*/
	struct NoiseMapInfo {
		PerlinNoiseType			type;
		std::optional<uint32_t>	seed;
		const std::size_t		width;
		const std::size_t		height;
		const std::size_t		depth;
		const std::size_t		layers;
		const float				frequency_0;
		const float				frequency_mult;
		const float				amplitude_mult;
	};

	/**
	 * @brief Contains the vertices, normals and vertex indices of a mesh,
	 * generated from a perlin noise map.
	*/
	struct PerlinMesh {
		Vect3						origin;
		std::vector<scop::Vertex>	vertices;
		std::vector<uint32_t>		indices;

		// std::vector<Vect3>		vertices;
		// std::vector<uint32_t>	indices;
		// std::vector<Vect3>		normals;
		// std::vector<Vect2>		uvs;
		// std::vector<int32_t>	texture_indices;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	PerlinNoise(NoiseMapInfo info);

	PerlinNoise(PerlinNoise&& other) = default;
	~PerlinNoise() = default;

	PerlinNoise() = delete;
	PerlinNoise(const PerlinNoise& other) = delete;
	PerlinNoise& operator=(PerlinNoise&& other) = delete;
	PerlinNoise& operator=(const PerlinNoise& other) = delete;

	/* ========================================================================= */

	std::vector<uint32_t>		toPixels() const;
	PerlinMesh					toMesh() const;

	/* GETTERS ================================================================= */

	std::size_t					getWidth() const noexcept;
	std::size_t					getHeight() const noexcept;
	uint32_t					getSeed() const noexcept;
	const std::vector<float>&	getNoiseMap() const noexcept;

private:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	/**
	 * Size of the random values table.
	 * Multiple of 2 to allow for bitwise AND insteand of modulo.
	*/
	static constexpr const std::size_t	table_sizes = 2 << 8;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	const uint32_t				seed;
	const std::size_t			width;
	const std::size_t			height;
	const std::size_t			depth;
	const std::size_t			layers;
	const float					frequency;
	const float					frequency_mult;
	const float					amplitude_mult;
	std::mt19937				generator;

	std::vector<std::size_t>	permutation_table;
	std::vector<float>			noise_map;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	uint32_t					generateSeed() const;
	float						generateFloat(float min, float max);
	std::vector<float>			generateRandomTable();
	std::vector<scop::Vect3>	generateGradientTable();
	std::vector<std::size_t>	generatePermutationTable();

	template <typename T>
	float						evaluateAt(
		const T& x,
		std::function<T(const T&)> floorFn,
		std::function<T(const T&, int32_t)> modFn,
		std::function<float(T, T, T)> lerpFn,
		const T unit
	) const;

	std::size_t					hash(
		const float x
	) const noexcept;
	std::size_t					hash(
		const float x,
		const float y
	) const noexcept;
	std::size_t					hash(
		const float x,
		const float y,
		const float z
	) const noexcept;

	/* ========================================================================= */

	std::vector<float>			generate1dNoiseMap();
	std::vector<float>			generate2dNoiseMap();
	std::vector<float>			generate3dNoiseMap();

}; // class PerlinNoise

} // namespace vox