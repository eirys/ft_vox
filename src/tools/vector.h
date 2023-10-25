/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:16:07 by etran             #+#    #+#             */
/*   Updated: 2023/06/07 02:29:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <cmath> // std::fma, std::sqrt
# include <stdexcept> // std::out_of_range
# include <ostream> // std::ostream

namespace scop {

template <class Vector>
Vector	operator*(float lhs, const Vector& rhs) noexcept {
	return rhs.operator*(lhs);
}

template <class Vector>
Vector	operator/(float lhs, const Vector& rhs) noexcept {
	return rhs.operator/(lhs);
}

struct Vect2 {
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	union {
		struct { float x, y; };
		struct { float r, g; };
		struct { float u, v; };
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	constexpr Vect2(): x(0.0f), y(0.0f) {}
	constexpr Vect2(float value): x(value), y(value) {}
	constexpr Vect2(float x, float y): x(x), y(y) {}

	constexpr Vect2(const Vect2& other) = default;
	constexpr Vect2& operator=(const Vect2& rhs) = default;
	constexpr Vect2& operator=(Vect2&& rhs) = default;
	~Vect2() = default;

	/* ACCESSORS =============================================================== */

	constexpr float&		operator[](std::size_t index);
	constexpr const float&	operator[](std::size_t index) const;

	/* OPERATORS =============================================================== */

	constexpr bool		operator!() const noexcept;
	constexpr Vect2		operator-() const noexcept;
	constexpr Vect2&	operator+=(const Vect2& rhs) noexcept;
	constexpr Vect2		operator+(const Vect2& rhs) const noexcept;
	constexpr Vect2&	operator-=(const Vect2& rhs) noexcept;
	constexpr Vect2		operator-(const Vect2& rhs) const noexcept;
	constexpr Vect2&	operator*=(float rhs) noexcept;
	constexpr Vect2		operator*(float rhs) const noexcept;
	constexpr Vect2&	operator/=(float rhs) noexcept;
	constexpr Vect2		operator/(float rhs) const noexcept;

	/* BOOLEAN COMPARISON ====================================================== */

	constexpr bool	operator==(const Vect2& rhs) const noexcept;

}; // struct Vect2

struct Vect3 {

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float u, v, w; };
		Vect2 xy;
		Vect2 uv;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	constexpr Vect3(): x(0.0f), y(0.0f), z(0.0f) {}
	constexpr Vect3(float value): x(value), y(value), z(value) {}
	constexpr Vect3(float new_x, float new_y, float new_z):
		x(new_x), y(new_y), z(new_z) {}

	constexpr Vect3(const Vect3& other) = default;
	constexpr Vect3(Vect3&& other) = default;
	constexpr Vect3& operator=(const Vect3& rhs) = default;
	constexpr Vect3& operator=(Vect3&& rhs) = default;
	~Vect3() = default;

	/* ACCESSORS =============================================================== */

	constexpr const float&	operator[](std::size_t index) const;
	constexpr float&		operator[](std::size_t index);

	/* OPERATORS =============================================================== */

	constexpr bool		operator!() const noexcept;
	constexpr Vect3		operator-() const noexcept;
	constexpr Vect3&	operator+=(const Vect3& rhs) noexcept;
	constexpr Vect3		operator+(const Vect3& rhs) const noexcept;
	constexpr Vect3&	operator-=(const Vect3& rhs) noexcept;
	constexpr Vect3		operator-(const Vect3& rhs) const noexcept;
	constexpr Vect3&	operator*=(float rhs) noexcept;
	constexpr Vect3		operator*(float rhs) const noexcept;
	constexpr Vect3&	operator/=(float rhs) noexcept;
	constexpr Vect3		operator/(float rhs) const noexcept;

	/* BOOLEAN COMPARISON ====================================================== */

	constexpr bool	operator==(const Vect3& rhs) const noexcept;

}; // struct Vect3

struct Vect4 {
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	union {
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		Vect3 xyz;
		Vect3 rgb;
		Vect2 xy;
		Vect2 rg;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	constexpr Vect4(): x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	constexpr Vect4(float value): x(value), y(value), z(value), w(value) {}
	constexpr Vect4(float new_x, float new_y, float new_z, float new_w):
		x(new_x), y(new_y), z(new_z), w(new_w) {}

	constexpr Vect4(const Vect4& other) = default;
	constexpr Vect4(Vect4&& other) = default;
	constexpr Vect4& operator=(const Vect4& rhs) = default;
	constexpr Vect4& operator=(Vect4&& rhs) = default;
	~Vect4() = default;

	/* ACCESSORS =============================================================== */

	constexpr const float&	operator[](std::size_t index) const;
	constexpr float&		operator[](std::size_t index);

	/* OPERATORS =============================================================== */

	constexpr bool		operator!() const noexcept;
	constexpr Vect4		operator-() const noexcept;
	constexpr Vect4&	operator+=(const Vect4& rhs) noexcept;
	constexpr Vect4		operator+(const Vect4& rhs) const noexcept;
	constexpr Vect4&	operator-=(const Vect4& rhs) noexcept;
	constexpr Vect4		operator-(const Vect4& rhs) const noexcept;
	constexpr Vect4&	operator*=(float rhs) noexcept;
	constexpr Vect4		operator*(float rhs) const noexcept;
	constexpr Vect4&	operator/=(float rhs) noexcept;
	constexpr Vect4		operator/(float rhs) const noexcept;

	/* BOOLEAN COMPARISON ====================================================== */

	constexpr bool	operator==(const Vect4& rhs) const noexcept;

}; // struct Vect4

/* ========================================================================== */
/*                                                                            */
/*                                    VECT2                                   */
/*                                                                            */
/* ========================================================================== */

/* ACCESSORS =============================================================== */

constexpr
float&	Vect2::operator[](std::size_t index) {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		default:
			throw std::out_of_range("Matrix index out of range");
	}
}

constexpr
const float&	Vect2::operator[](std::size_t index) const {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		default:
			throw std::out_of_range("Matrix index out of range");
	}
}

/* OPERATORS =============================================================== */

constexpr
bool	Vect2::operator!() const noexcept {
	return !x && !y;
}

constexpr
Vect2	Vect2::operator-() const noexcept {
	return Vect2{ -x, -y };
}

constexpr
Vect2&	Vect2::operator+=(const Vect2& rhs) noexcept {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

constexpr
Vect2	Vect2::operator+(const Vect2& rhs) const noexcept {
	Vect2	res(*this);
	return res.operator+=(rhs);
}

constexpr
Vect2&	Vect2::operator-=(const Vect2& rhs) noexcept {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

constexpr
Vect2	Vect2::operator-(const Vect2& rhs) const noexcept {
	Vect2	res(*this);
	return res.operator-=(rhs);
}

constexpr
Vect2&	Vect2::operator*=(float rhs) noexcept {
	x *= rhs;
	y *= rhs;
	return *this;
}

constexpr
Vect2	Vect2::operator*(float rhs) const noexcept {
	Vect2	res(*this);
	return res.operator*=(rhs);
}

constexpr
Vect2&	Vect2::operator/=(float rhs) noexcept {
	x /= rhs;
	y /= rhs;
	return *this;
}

constexpr
Vect2	Vect2::operator/(float rhs) const noexcept {
	Vect2	res(*this);
	return res.operator/=(rhs);
}

/* BOOLEAN COMPARISON ====================================================== */

constexpr
bool	Vect2::operator==(const Vect2& rhs) const noexcept {
	return x == rhs.x && y == rhs.y;
}

/* ========================================================================== */

/**
 * @brief Returns the dot product of the vector with another vector
 */
inline float	dot(const Vect2& lhs, const Vect2& rhs) noexcept {
	return std::fma(
		lhs.x,
		rhs.x,
		std::fma(lhs.y, rhs.y, 0.0f));
}

/**
 * @brief Returns the norm of the vector
*/
inline float	norm(const Vect2& vec) noexcept {
	return std::sqrt(dot(vec, vec));
}

/**
 * @brief Returns a normalized vector
*/
inline Vect2	normalize(const Vect2& vec) noexcept {
	float	n = norm(vec);
	return Vect2{ vec.x / n, vec.y / n };
}

/* ========================================================================== */
/*                                                                            */
/*                                    VECT3                                   */
/*                                                                            */
/* ========================================================================== */

/* ACCESSORS =============================================================== */

constexpr
const float& Vect3::operator[](std::size_t index) const {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range("Matrix index out of range");
	}
}

constexpr
float& Vect3::operator[](std::size_t index) {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range("Matrix index out of range");
	}
}

/* OPERATORS =============================================================== */

constexpr
bool Vect3::operator!() const noexcept {
	return !x && !y && !z;
}

constexpr
Vect3	Vect3::operator-() const noexcept {
	return Vect3(-x, -y, -z);
}

constexpr
Vect3&	Vect3::operator+=(const Vect3& rhs) noexcept {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

constexpr
Vect3	Vect3::operator+(const Vect3& rhs) const noexcept {
	Vect3	res(*this);
	return res.operator+=(rhs);
}

constexpr
Vect3&	Vect3::operator-=(const Vect3& rhs) noexcept {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

constexpr
Vect3	Vect3::operator-(const Vect3& rhs) const noexcept {
	Vect3	res(*this);
	return res.operator-=(rhs);
}

constexpr
Vect3&	Vect3::operator*=(float rhs) noexcept {
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

constexpr
Vect3	Vect3::operator*(float rhs) const noexcept {
	Vect3	res(*this);
	return res.operator*=(rhs);
}

constexpr
Vect3&	Vect3::operator/=(float rhs) noexcept {
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

constexpr
Vect3	Vect3::operator/(float rhs) const noexcept {
	Vect3	res(*this);
	return res.operator/=(rhs);
}

/* BOOLEAN COMPARISON ====================================================== */

constexpr
bool	Vect3::operator==(const Vect3& rhs) const noexcept {
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

/* ========================================================================== */

/**
 * @brief Returns the dot product of the vector with another vector
 */
inline float	dot(const Vect3& lhs, const Vect3& rhs) noexcept {
	return std::fma(
		lhs.x,
		rhs.x,
		std::fma(lhs.y, rhs.y, std::fma(lhs.z, rhs.z, 0.0f))
	);
}

/**
 * @brief Returns the norm of the vector
 */
inline float	norm(const Vect3& vec) noexcept {
	return std::sqrt(dot(vec, vec));
}

/**
 * @brief Returns a normalized vector
 */
inline Vect3	normalize(const Vect3& vec) noexcept {
	float	n = norm(vec);
	return Vect3{ vec.x / n, vec.y / n, vec.z / n };
}

/**
 * @brief Returns the cross product of the vector with another vector
 */
inline Vect3	cross(const Vect3& lhs, const Vect3& rhs) noexcept {
	return Vect3(
		std::fma(lhs.y, rhs.z, std::fma(-lhs.z, rhs.y, 0.0f)),
		std::fma(lhs.z, rhs.x, std::fma(-lhs.x, rhs.z, 0.0f)),
		std::fma(lhs.x, rhs.y, std::fma(-lhs.y, rhs.x, 0.0f))
	);
}

/**
 * @brief Special fma for Vect3 and float: a * b + c.
 *
 * @param a		The vector to multiply.
 * @param b		The scalar to multiply.
 * @param c		The vector to add.
*/
inline Vect3	fma(const Vect3& a, float b, const Vect3& c) noexcept {
	return Vect3(
		std::fma(a.x, b, c.x),
		std::fma(a.y, b, c.y),
		std::fma(a.z, b, c.z)
	);
}

/* ========================================================================== */
/*                                                                            */
/*                                    VECT4                                   */
/*                                                                            */
/* ========================================================================== */

/* ACCESSORS =============================================================== */

constexpr
const float& Vect4::operator[](std::size_t index) const {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			throw std::out_of_range("Matrix index out of range");
	}
}

constexpr
float& Vect4::operator[](std::size_t index) {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			throw std::out_of_range("Matrix index out of range");
	}
}

/* OPERATORS =============================================================== */

constexpr
bool Vect4::operator!() const noexcept {
	return !x && !y && !z && !w;
}

constexpr
Vect4	Vect4::operator-() const noexcept {
	return Vect4(-x, -y, -z, -w);
}

constexpr
Vect4&	Vect4::operator+=(const Vect4& rhs) noexcept {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

constexpr
Vect4	Vect4::operator+(const Vect4& rhs) const noexcept {
	Vect4	res(*this);
	return res.operator+=(rhs);
}

constexpr
Vect4&	Vect4::operator-=(const Vect4& rhs) noexcept {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

constexpr
Vect4	Vect4::operator-(const Vect4& rhs) const noexcept {
	Vect4	res(*this);
	return res.operator-=(rhs);
}

constexpr
Vect4&	Vect4::operator*=(float rhs) noexcept {
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
	return *this;
}

constexpr
Vect4	Vect4::operator*(float rhs) const noexcept {
	Vect4	res(*this);
	return res.operator*=(rhs);
}

constexpr
Vect4&	Vect4::operator/=(float rhs) noexcept {
	x /= rhs;
	y /= rhs;
	z /= rhs;
	w /= rhs;
	return *this;
}

constexpr
Vect4	Vect4::operator/(float rhs) const noexcept {
	Vect4	res(*this);
	return res.operator/=(rhs);
}

/* BOOLEAN COMPARISON ====================================================== */

constexpr
bool	Vect4::operator==(const Vect4& rhs) const noexcept {
	return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

/* ========================================================================== */

/**
 * @brief Returns the dot product of the vector with another vector
 */
inline float	dot(const Vect4& lhs, const Vect4& rhs) noexcept {
	return std::fma(
		lhs.x,
		rhs.x,
		std::fma(
			lhs.y,
			rhs.y,
			std::fma(lhs.z, rhs.z, std::fma(lhs.w, rhs.w, 0.0f)))
	);
}

/**
 * @brief Returns the norm of the vector
 */
inline float	norm(const Vect4& vec) noexcept {
	return std::sqrt(dot(vec, vec));
}

/**
 * @brief Returns a normalized vector
 */
inline Vect4	normalize(const Vect4& vec) noexcept {
	float	n = norm(vec);
	return Vect4{ vec.x / n, vec.y / n, vec.z / n, vec.w / n };
}

/**
 * @brief Special fma for Vect4 and float: a * b + c.
 *
 * @param a		The vector to multiply.
 * @param b		The scalar to multiply.
 * @param c		The vector to add.
*/
inline Vect4	fma(const Vect4& a, float b, const Vect4& c) noexcept {
	return Vect4(
		std::fma(a.x, b, c.x),
		std::fma(a.y, b, c.y),
		std::fma(a.z, b, c.z),
		std::fma(a.w, b, c.w)
	);
}

} // namespace scop

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

/* HASH FUNCTIONS =========================================================== */

template<>
struct std::hash<scop::Vect3> {
	inline std::size_t	operator()(const scop::Vect3& vect) const {
		return (
			std::hash<float>()(vect.x) ^
			std::hash<float>()(vect.y) ^
			std::hash<float>()(vect.z)
		);
	}
};

template<>
struct std::hash<scop::Vect2> {
	inline std::size_t	operator()(const scop::Vect2& vect) const {
		return (
			std::hash<float>()(vect.x) ^
			std::hash<float>()(vect.y)
		);
	}
};

/* IOSTREAM ================================================================= */

inline std::ostream& operator<<(std::ostream& os, const scop::Vect3& vect) {
	os << "{x:" << vect.x << ", y:" << vect.y << ", z:" << vect.z << "}";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const scop::Vect2& vect) {
	os << "{x:" << vect.x << ", y:" << vect.y << "}";
	return os;
}