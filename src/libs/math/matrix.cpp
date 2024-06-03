/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 23:18:11 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 19:10:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

#include <cstring> // memset

namespace math {

/* ========================================================================== */
/*                                 4X4 MATRIX                                 */
/* ========================================================================== */

Mat4::Mat4() {
	for (std::size_t i = 0; i < 16; ++i) {
	// memset(mat, 0, 16 * sizeof(float));
		mat[i] = 0.0f;
	}
}

Mat4::Mat4(const Mat4& other) {
	for (std::size_t i = 0; i < 16; ++i) {
		mat[i] = other.mat[i];
	}
}

Mat4::Mat4(float x) {
	for (std::size_t i = 0; i < 16; ++i) {
		if (i % 5 == 0)
			mat[i] = x;
		else
			mat[i] = 0;
	}
}

Mat4::Mat4(std::initializer_list<float> list) {
	if (list.size() != 16) {
		throw std::invalid_argument("Mat4 initializer list must have 16 elements");
	}
	for (std::size_t i = 0; i < list.size(); ++i) {
		mat[i] = *(list.begin() + i);
	}
}

Mat4&	Mat4::operator=(const Mat4& rhs) noexcept {
	for (std::size_t i = 0; i < 16; ++i) {
		mat[i] = rhs.mat[i];
	}
	return *this;
}

/* ACCESSORS =============================================================== */

constexpr float&	Mat4::operator[](std::size_t index) {
	if (index >= 16) {
		throw std::out_of_range("Matrix index out of range");
	}
	return mat[index];
}

constexpr float	Mat4::operator[](std::size_t index) const {
	if (index >= 16) {
		throw std::out_of_range("Matrix index out of range");
	}
	return mat[index];
}

/* OPERATORS =============================================================== */

Mat4&	Mat4::operator+=(const Mat4& rhs) noexcept {
	for (std::size_t i = 0; i < 16; i++) {
		mat[i] += rhs.mat[i];
	}
	return *this;
}

Mat4	Mat4::operator+(const Mat4& rhs) const noexcept {
	Mat4	result(*this);
	return result.operator+=(rhs);
}

Mat4&	Mat4::operator-=(const Mat4& rhs) noexcept {
	for (std::size_t i = 0; i < 16; i++) {
		mat[i] -= rhs.mat[i];
	}
	return *this;
}

Mat4	Mat4::operator-(const Mat4& rhs) const noexcept {
	Mat4	result(*this);
	return result.operator-=(rhs);
}

Mat4&	Mat4::operator*=(const Mat4& rhs) noexcept {
	Mat4	result;
	for (std::size_t i = 0; i < 4; i++) {
		for (std::size_t j = 0; j < 4; j++) {
			for (std::size_t k = 0; k < 4; k++) {
				result[i * 4 + j] = static_cast<float>(
					std::fma(mat[k * 4 + j], rhs.mat[i * 4 + k], result[i * 4 + j])
				);
			}
		}
	}
	*this = result;
	return *this;
}

Mat4	Mat4::operator*(const Mat4& rhs) const noexcept {
	Mat4 result(*this);
	return result.operator*=(rhs);
}

/* ========================================================================= */

Mat4&	Mat4::operator*=(float rhs) noexcept {
	for (std::size_t i = 0; i < 16; i++) {
		mat[i] *= rhs;
	}
	return *this;
}

Mat4	Mat4::operator*(float rhs) const noexcept {
	Mat4 result(*this);
	return result.operator*=(rhs);
}

Vect3	Mat4::operator*(const Vect3& rhs) const noexcept {
	Vect3 result;

	result.x = static_cast<float>(
		std::fma(
			mat[0],
			rhs.x,
			std::fma(mat[4], rhs.y, std::fma(mat[8], rhs.z, mat[12]))
		)
	);
	result.y = static_cast<float>(
		std::fma(
			mat[1],
			rhs.x,
			std::fma(mat[5], rhs.y, std::fma(mat[9], rhs.z, mat[13]))
		)
	);
	result.z = static_cast<float>(
		std::fma(
			mat[2],
			rhs.x,
			std::fma(mat[6], rhs.y, std::fma(mat[10], rhs.z, mat[14]))
		)
	);
	return result;
}

/**
 * @brief Compute the minor [row, column] of the matrix.
 *
 * @param row		The line to remove.
 * @param column	The column to remove.
*/
Mat3	Mat4::minor(std::size_t row, std::size_t column) const {
	Mat3	submatrix{};
	std::size_t	x = 0;
	std::size_t	y = 0;

	for (std::size_t line = 0; line < 4; ++line) {
		if (line != row) {
			for (std::size_t col = 0; col < 4; ++col) {
				if (col != column) {
					submatrix[y * 3 + x] = mat[line * 3 + col];
					++x;
				}
			}
			++y;
		}
		x = 0;
	}
	return submatrix;
}

/**
 * @brief Compute the determinant of a 4x4 matrix.
*/
float	Mat4::det() const {
	float	sum{};

	for (std::size_t j = 0; j < 4; ++j) {
		sum = std::fma(
			static_cast<float>(std::pow(-1.0f, j)),
			std::fma(
				minor(0, j).det(),
				mat[j],
				0.0f
			),
			sum
		);
	}
	return sum;
}

Mat4	Mat4::adjugate() const {
	Mat4	cofactor{};

	for (std::size_t i = 0; i < 4; ++i) {
		for (std::size_t j = 0; j < 4; ++j) {
			cofactor[4 * i + j] = std::fma(
				static_cast<float>(std::pow(-1.0f, i + j)),
				minor(i, j).det(),
				0.0f
			);
		}
	}
	return cofactor.transpose();
}

Mat4	Mat4::transpose() const {
	Mat4	result{};

	for (std::size_t i = 0; i < 4; ++i) {
		for (std::size_t j = 0; j < 4; ++j) {
			result[4 * i + j] = mat[4 * j + i];
		}
	}
	return result;
}

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

/**
 * @brief Produces rotation matrix around the given axis
 *
 * @param mat	matrix to rotate
 * @param angle	angle in radians
 * @param axis	axis of rotation
 *
 * @details Result:
 * [[u.x * u.x * (1 - c) + c,			col 0
 *	 u.x * u.y * (1 - c) - u.z * s,
 *	 u.x * u.z * (1 - c) + u.y * s,
 *	 0],
 *  [u.y * u.x * (1 - c) + u.z * s,		col 1
 *	 u.y * u.y * (1 - c) + c,
 *	 u.y * u.z * (1 - c) - u.x * s,
 *	 0],
 *  [u.z * u.x * (1 - c) - u.y * s,		col 2
 *	 u.z * u.y * (1 - c) + u.x * s,
 *	 u.z * u.z * (1 - c) + c,
 *	 0],
 *	[0, 0, 0, 1]]						col 3
*/
Mat4	rotate(const Mat4& mat, float angle, const Vect3& axis) noexcept {
	const float	c = std::cos(angle);
	const float	s = std::sin(angle);
	const Vect3	u = math::normalize(axis);

	return mat * Mat4{
		// Col 1
		static_cast<float>(std::fma(u.x, std::fma(u.x, 1 - c, 0), c)),
		static_cast<float>(std::fma(u.x, std::fma(u.y, 1 - c, 0), std::fma(-u.z, s, 0))),
		static_cast<float>(std::fma(u.x, std::fma(u.z, 1 - c, 0), std::fma(u.y, s, 0))),
		0,
		// Col 2
		static_cast<float>(std::fma(u.y, std::fma(u.x, 1 - c, 0), std::fma(u.z, s, 0))),
		static_cast<float>(std::fma(u.y, std::fma(u.y, 1 - c, 0), c)),
		static_cast<float>(std::fma(u.y, std::fma(u.z, 1 - c, 0), std::fma(-u.x, s, 0))),
		0,
		// Col 3
		static_cast<float>(std::fma(u.z, std::fma(u.x, 1 - c, 0), std::fma(-u.y, s, 0))),
		static_cast<float>(std::fma(u.z, std::fma(u.y, 1 - c, 0), std::fma(u.x, s, 0))),
		static_cast<float>(std::fma(u.z, std::fma(u.z, 1 - c, 0), c)),
		0,
		// Col 4
		0, 0, 0, 1
	};
}

/**
 * @brief Produces lookAt matrix
 *
 * @param eyePos:		position of the camera
 * @param center:	position of the object to look at
 * @param world_up:	world up vector, usually (0, 0, 1). Used to determine
 * 					orientation of the camera. Should not be parallel
 * 					to the vector from eye to center.
*/
Mat4	lookAt(const Vect3& eyePos, const Vect3& center, const Vect3& world_up) noexcept {
	const Vect3	forward = math::normalize(center - eyePos);
	const Vect3	right = math::normalize(math::cross(forward, world_up));
	const Vect3	up = math::cross(right, forward);

	return Mat4{
		// Col 1
		right.x, up.x, -forward.x, 0,
		// Col 2
		right.y, up.y, -forward.y, 0,
		// Col 3
		right.z, up.z, -forward.z, 0,
		// Col 4
		math::dot(-right, eyePos), math::dot(-up, eyePos), math::dot(forward, eyePos), 1
	};
}

Mat4	lookAt(
	const Vect3& eyePos,
	const Vect3& cam_front,
	const Vect3& cam_up,
	const Vect3& cam_right
) noexcept {
	return Mat4 {
		// Col 1
		cam_right.x, cam_up.x, -cam_front.x, 0,
		// Col 2
		cam_right.y, cam_up.y, -cam_front.y, 0,
		// Col 3
		cam_right.z, cam_up.z, -cam_front.z, 0,
		// Col 4
		math::dot(-cam_right, eyePos), math::dot(-cam_up, eyePos), math::dot(cam_front, eyePos), 1
	};
}

/**
 * @brief Produces perspective projection matrix
 *
 * @param fov			field of view in radians
 * @param aspect_ratio	aspect ratio of the screen
 * @param near			near clipping plane
 * @param far			far clipping plane
*/
Mat4	perspective(float fov, float aspect_ratio, float near, float far) noexcept {
	const float	tanHalfFov = std::tan(fov / 2);
	const float	range = far - near;

	return Mat4{
		// Col 1
		1 / (aspect_ratio * tanHalfFov), 0, 0, 0,
		// Col 2
		0, -1 / tanHalfFov, 0, 0,
		// Col 3
		0, 0, -far / range, -1,
		// Col 4
		0, 0, -(far * near) / range, 0
	};
}

Mat4	orthographic(
	float left,
	float right,
	float bot,
	float top,
	float near,
	float far
) noexcept {
	const float width = right - left;
	const float height = bot - top;
	const float	range = near - far;
	return Mat4{
		// Col 1
		2 / width, 0, 0, 0,
		// Col 2
		0, -2 / height, 0, 0,
		// Col 3
		0, 0, 1.0f / (range), 0,
		// Col 4
		-(right + left) / width, -(top + bot) / height, near / range, 1 };
}

/**
 * @brief Scales the matrix by the given vector
 *
 * @param mat:		matrix to scale
 * @param scale:	vector to scale by
 *
 * @note Only scales the first 3 rows of the matrix
*/
Mat4	scale(const Mat4& mat, const Vect3& scale) noexcept {
	Mat4	result(mat);

	for (std::size_t j = 0; j < 3; ++j) {
		for (std::size_t i = 0; i < 3; ++i) {
			result[j * 4 + i] *= scale[j];
		}
	}
	return result;
}

/**
 * @brief Produces a translation matrix from the given vector
 *
 * @param mat	matrix to translate
 * @param dir	vector to translate by
 *
 * @details Result:
 *
 * for (uint32_t i = 0; i < 4; ++i)
 *  result[12 + i] = (mat[i] * dir.x) + (mat[4 + i] * dir.y) + (mat[8 + i] * dir.z) + mat[12 + i];
*/
Mat4	translate(const Mat4& mat, const Vect3& dir) noexcept {
	Mat4	result(mat);
    for (uint32_t i = 0; i < 4; ++i)
        result[12 + i] = std::fma(
            mat[i],
            dir.x,
            std::fma(mat[4 + i], dir.y, std::fma(mat[8 + i], dir.z, mat[12 + i])));

	return result;
}

/**
 * @brief Compute the inverse of a 4x4 matrix.
*/
Mat4	inverse(const Mat4& mat) {
	const float determinant = mat.det();
	if (!determinant) {
		// The matrix is singular.
		throw std::invalid_argument("Attempt to inverse a singular matrix.");
	}

	// Calculate the adjugate matrix.
	Mat4	adjugate = mat.adjugate();
	return adjugate * (1 / determinant);
}

/* ========================================================================== */
/*                                 MATRIX 3X3                                 */
/* ========================================================================== */

Mat3::Mat3() {
	memset(mat, 0, 9 * sizeof(float));
}

/* ACCESSORS ================================================================ */

float&	Mat3::operator[](std::size_t index) {
	if (index >= 9) {
		throw std::out_of_range("Mat3 index out of range");
	}
	return mat[index];
}

const float&	Mat3::operator[](std::size_t index) const {
	if (index >= 9) {
		throw std::out_of_range("Mat3 index out of range");
	}
	return mat[index];
}

/* ========================================================================== */

Mat2	Mat3::minor(std::size_t row, std::size_t column) const {
	Mat2	submatrix{};
	std::size_t	x = 0;
	std::size_t	y = 0;

	for (std::size_t line = 0; line < 3; ++line) {
		if (line != row) {
			for (std::size_t col = 0; col < 3; ++col) {
				if (col != column) {
					submatrix[2 * y + x] = mat[2 * line + col];
					++x;
				}
			}
			++y;
		}
		x = 0;
	}
	return submatrix;
}

float	Mat3::det() const {
	float	sum{};

	for (std::size_t j = 0; j < 3; ++j) {
		sum = std::fma(
			static_cast<float>(std::pow(-1.0f, j)),
			std::fma(
				minor(0, j).det(),
				mat[j],
				0.0f
			),
			sum
		);
	}
	return sum;
}

/* ========================================================================== */
/*                                 MATRIX 2X2                                 */
/* ========================================================================== */

Mat2::Mat2() {
	memset(mat, 0, 4 * sizeof(float));
}

/* ACCESSORS ================================================================ */

float& Mat2::operator[](std::size_t index) {
	if (index >= 4) {
		throw std::out_of_range("Mat2 index out of range");
	}
	return mat[index];
}

const float& Mat2::operator[](std::size_t index) const {
	if (index >= 4) {
		throw std::out_of_range("Mat2 index out of range");
	}
	return mat[index];
}

/* ========================================================================== */

float	Mat2::det() const {
	return std::fma(mat[0], mat[3], -std::fma(mat[1], mat[2], 0.0f));
}

} // namespace math