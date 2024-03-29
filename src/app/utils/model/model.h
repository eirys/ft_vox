/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 19:23:53 by eli               #+#    #+#             */
/*   Updated: 2023/06/03 12:48:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <vector> // std::vector
# include <array> // std::array
# include <stdexcept> // std::out_of_range

# include "material.h"

# define SCOP_TEXTURE_FILE_DEFAULT "assets/textures/grass_block.ppm"

namespace scop {
class Image;
struct Vertex;

namespace obj {

/**
 * Contains .obj file data.
*/
class Model {
public:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	/**
	 * Contains vertex, texture and normal indices of a set. (Face parsing)
	*/
	struct Index {
		int							vertex;
		int							texture;
		int							normal;

		int	operator[](std::size_t i) const {
			switch (i) {
				case 0: return vertex;
				case 1: return texture;
				case 2: return normal;
				default: throw std::out_of_range("Index out of range");
			}
		}
		int&	operator[](std::size_t i) {
			switch (i) {
				case 0: return vertex;
				case 1: return texture;
				case 2: return normal;
				default: throw std::out_of_range("Index out of range");
			}
		}
	};

	/**
	 * Contains 3 sets of indices of a triangle.
	*/
	struct Triangle {
		std::array<Index, 3>		indices;

		const Index&	operator[](std::size_t i) const {
			if (i >= 3)
				throw std::out_of_range("Index out of range");
			return indices[i];
		}
		Index&	operator[](std::size_t i) {
			if (i >= 3)
				throw std::out_of_range("Index out of range");
			return indices[i];
		}
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Model() = default;
	Model(Model&& x) = default;
	Model&	operator=(Model&& x) = default;
	~Model() = default;

	Model(const Model& x) = delete;
	Model&	operator=(const Model& x) = delete;

	/* ========================================================================= */

	void							addVertex(const Vect3& vertex);
	void							addTexture(const Vect2& texture);
	void							addNormal(const Vect3& normal);
	void							addIndex(const Index& index);
	void							addTriangle(const Triangle& triangle);

	void							reserveVertices(const std::size_t size);
	void							reserveTriangles(const std::size_t size);

	void							setDefaultTextureCoords();
	void							setDefaultNormalCoords();

	void							setMaterial(mtl::Material&& material);
	void							toggleSmoothShading() noexcept;

	const std::vector<Vect3>&		getVertexCoords() const noexcept;
	const std::vector<Vect2>&		getTextureCoords() const noexcept;
	const std::vector<Vect3>&		getNormalCoords() const noexcept;
	const std::vector<Index>&		getIndices() const noexcept;
	const std::vector<Triangle>&	getTriangles() const noexcept;
	const mtl::Material&			getMaterial() const noexcept;
	mtl::Material&					getMaterial() noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	std::vector<scop::Vect3>		vertex_coords;
	std::vector<scop::Vect2>		texture_coords;
	std::vector<scop::Vect3>		normal_coords;
	std::vector<Index>				indices;
	std::vector<Triangle>			triangles;
	mtl::Material					material{};
	bool							smooth_shading = false;

}; // class Model

} // namespace obj
} // namespace scop