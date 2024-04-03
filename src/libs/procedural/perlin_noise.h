/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:13:52 by etran             #+#    #+#             */
/*   Updated: 2024/04/02 15:43:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector> // std::vector
# include <functional> // std::function
# include <random> // std::mt19937
# include <optional> // std::optional

# include "types.h"
# include "vector.h"

namespace proc {

using Vect2 = math::Vect2;
using Vect3 = math::Vect3;

enum class PerlinNoiseType {
	PERLIN_NOISE_1D,
	PERLIN_NOISE_2D,
	PERLIN_NOISE_3D
};

/* ========================================================================== */
/*                               HELPER OBJECTS                               */
/* ========================================================================== */

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
 *
 * @param scale
*/
struct NoiseMapInfo {
    PerlinNoiseType			type;
    std::optional<uint32_t>	seed;
    std::size_t				width;
    std::size_t				height;
    std::size_t				depth;
    std::size_t				layers;
    float					frequency_0;
    float					frequency_mult;
    float					amplitude_mult;
    float                   scale = 16;
    float                   shift = 0;
};

/**
 * Perlin noise map generator.
*/
class PerlinNoise {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	PerlinNoise(const NoiseMapInfo& info);

	PerlinNoise(PerlinNoise&& other) = default;
	~PerlinNoise() = default;

	PerlinNoise() = delete;
	PerlinNoise(const PerlinNoise& other) = delete;
	PerlinNoise& operator=(const PerlinNoise& other) = delete;

	/* ========================================================================= */

	std::vector<uint32_t>		toPixels() const;
    void                        copyToBuffer(u32* buffer) const;
    void                        debug(const std::string& outfile) const;

	float						noiseAt(std::size_t x) const noexcept;
	float						noiseAt(
		std::size_t x,
		std::size_t y) const noexcept;
	float						noiseAt(
		std::size_t x,
		std::size_t y,
		std::size_t z) const noexcept;

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
	static constexpr            u32 table_sizes = 2 << 8;

	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	const uint32_t				seed;
	const std::size_t			width;
	const std::size_t			height;
	const std::size_t			depth;
	const std::size_t			layers;
	const float					frequency;
	const float					frequency_mult;
	const float					amplitude_mult;
	const float					scale;
	const float					shift;

	std::mt19937				generator;

	std::vector<std::size_t>	permutation_table;
	std::vector<float>			noise_map;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	uint32_t					_generateSeed() const;
	float						_generateFloat(float min, float max);
	std::vector<float>			_generateRandomTable();
	std::vector<Vect3>	        _generateGradientTable();
	std::vector<std::size_t>	_generatePermutationTable();

	template <typename T>
	float						_evaluateAt(
		const T& x,
		std::function<T(const T&)> floorFn,
		std::function<T(const T&, int32_t)> modFn,
		std::function<float(T, T, T)> lerpFn,
		const T unit
	) const;

	std::size_t					_hash(
		const float x
	) const noexcept;
	std::size_t					_hash(
		const float x,
		const float y
	) const noexcept;
	std::size_t					_hash(
		const float x,
		const float y,
		const float z
	) const noexcept;

	/* ========================================================================= */

	std::vector<float>			_generate1dNoiseMap();
	std::vector<float>			_generate2dNoiseMap();
	std::vector<float>			_generate3dNoiseMap();

}; // class PerlinNoise

} // namespace proc