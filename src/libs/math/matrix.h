/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:16:40 by etran             #+#    #+#             */
/*   Updated: 2024/06/04 16:06:16 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <stdexcept>
# include <cstring>

# include "vector.h"

namespace math {
struct Mat2;
struct Mat3;

struct Mat4 {
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	float	mat[16];

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Mat4();
	Mat4(const Mat4& other);
	explicit Mat4(float x);
	Mat4(std::initializer_list<float> list);
	Mat4&	operator=(const Mat4& rhs) noexcept;

	~Mat4() = default;

	/* ACCESSORS =============================================================== */

	float&	        operator[](std::size_t index);
	float 	        operator[](std::size_t index) const;

	/* OPERATORS =============================================================== */

	Mat4&			operator+=(const Mat4& rhs) noexcept;
	Mat4			operator+(const Mat4& rhs) const noexcept;
	Mat4&			operator-=(const Mat4& rhs) noexcept;
	Mat4			operator-(const Mat4& rhs) const noexcept;
	Mat4&			operator*=(const Mat4& rhs) noexcept;
	Mat4			operator*(const Mat4& rhs) const noexcept;

	/* ========================================================================= */

	Mat4&			operator*=(float rhs) noexcept;
	Mat4			operator*(float rhs) const noexcept;
	Vect3			operator*(const Vect3& rhs) const noexcept;

	Mat3			minor(std::size_t row, std::size_t col) const;
	float			det() const;
	Mat4			adjugate() const;
	Mat4			transpose() const;

}; // struct Mat4

Mat4	lookAt(const Vect3& eyePos, const Vect3& center, const Vect3& world_up) noexcept;
Mat4	lookAt(const Vect3& eyePos, const Vect3& cam_front, const Vect3& cam_up, const Vect3& cam_right) noexcept;
Mat4	perspective(float fov, float aspect_ratio, float near, float far) noexcept;
Mat4	orthographic(
			float bot,
			float top,
			float left,
			float right,
			float near,
			float far) noexcept;
Mat4	rotate(const Mat4& mat, float angle, const Vect3& axis) noexcept;
Mat4	scale(const Mat4& mat, const Vect3& scale) noexcept;
Mat4	translate(const Mat4& mat, const Vect3& dir) noexcept;
Mat4	inverse(const Mat4& mat);

/* ========================================================================== */
/*                                 UTILITARIES                                */
/* ========================================================================== */

struct Mat3 {

	float	mat[9];

	Mat3();

	Mat3(const Mat3& other) = default;
	Mat3(Mat3&& other) = default;
	~Mat3() = default;

	/* ACCESSORS =============================================================== */

	float&	operator[](std::size_t index);
	const float&	operator[](std::size_t index) const;

	/* ========================================================================= */

	Mat2	minor(std::size_t row, std::size_t col) const;
	float	det() const;

}; // struct Mat3

struct Mat2 {

	float	mat[4];

	Mat2();

	Mat2(const Mat2& other) = default;
	Mat2(Mat2&& other) = default;
	~Mat2() = default;

	/* ACCESSORS =============================================================== */

	float&	operator[](std::size_t index);
	const float&	operator[](std::size_t index) const;

	/* ========================================================================= */

	float	det() const;

}; // struct Mat2

}; // namespace math