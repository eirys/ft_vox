/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:15:29 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 12:55:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <memory> // std::unique_ptr

# include "vector.h"
# include "image_handler.h"

namespace scop {
namespace mtl {

enum IlluminationModel {
	ILLUM_NONE = 0,
	ILLUM_LAMBERTIAN = 1,
	ILLUM_BLINN_PHONG = 2,
	// ILLUM_OREN_NAYAR = 3,
	// ILLUM_MINNAERT = 4,
};

/**
 * Contains .mtl file data.
*/
struct Material {
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	typedef		std::unique_ptr<scop::Image>	ImagePtr;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */


	Material() = default;
	Material(Material&& other) = default;
	Material&	operator=(Material&& other) = default;
	~Material() = default;

	Material(const Material& other) = delete;
	Material&	operator=(const Material& other) = delete;

	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	// newmtl (material name)
	std::string			name;

	// Ka (ambient color)
	scop::Vect3			ambient_color = scop::Vect3(0.01f, 0.01f, 0.01f);

	// Kd (diffuse color)
	scop::Vect3			diffuse_color = scop::Vect3(1.0f, 1.0f, 0.7f);

	// Ks (specular color)
	scop::Vect3			specular_color{};

	// Ke (emissive coefficient)
	scop::Vect3			emissive_color{};

	// d or Tr (object opacity)
	float				opacity = 1.0f;

	// Ns (object shininess)
	int32_t				shininess = 0;

	// illum (illumination model)
	IlluminationModel	illum = IlluminationModel::ILLUM_LAMBERTIAN;

	// map_Ka (ambient texture)
	ImagePtr			ambient_texture = nullptr;

}; // class Material

} // namespace mtl
} // namespace scop

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

inline std::ostream& operator<<(
	std::ostream& o,
	const scop::mtl::Material& mat
) {
	o	<< "Material: " << mat.name << '\n'
		<< "  Ambient color: " << mat.ambient_color << '\n'
		<< "  Diffuse color: " << mat.diffuse_color << '\n'
		<< "  Specular color: " << mat.specular_color << '\n'
		<< "  Emissive color: " << mat.emissive_color << '\n'
		<< "  Opacity: " << mat.opacity << '\n'
		<< "  Shininess: " << static_cast<int>(mat.shininess) << '\n'
		<< "  Illumination model: " << static_cast<int>(mat.illum) << '\n'
		<< "  Ambient texture: " << mat.ambient_texture.get() << '\n';
	return o;
}