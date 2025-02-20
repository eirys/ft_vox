/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:16:07 by etran             #+#    #+#             */
/*   Updated: 2024/08/26 13:50:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <cmath> // std::fma, std::sqrt
# include <stdexcept> // std::out_of_range
# include <ostream> // std::ostream

namespace math {

template <typename T>
struct Vect2 {
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	union {
		struct { T x, y; };
		struct { T r, g; };
		struct { T u, v; };
		T elem[2];
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	constexpr Vect2(): x(T(0)), y(T(0)) {}
	constexpr Vect2(T value): x(value), y(value) {}
	constexpr Vect2(T x, T y): x(x), y(y) {}

	constexpr Vect2(const Vect2& other) = default;
	constexpr Vect2& operator=(const Vect2& rhs) = default;
	constexpr Vect2& operator=(Vect2&& rhs) = default;
	constexpr Vect2& operator=(T rhs) noexcept { x = rhs; y = rhs; return *this; }
	~Vect2() = default;

    template <typename U>
    explicit constexpr operator Vect2<U>() const noexcept {
        return Vect2<U>(static_cast<U>(x), static_cast<U>(y));
    }

	/* ACCESSORS =============================================================== */

	constexpr T&		operator[](std::size_t index);
	constexpr const T&	operator[](std::size_t index) const;

	/* OPERATORS =============================================================== */

	constexpr bool		operator!() const noexcept;
	constexpr Vect2		operator-() const noexcept;
	constexpr Vect2&	operator+=(const Vect2& rhs) noexcept;
	constexpr Vect2		operator+(const Vect2& rhs) const noexcept;
	constexpr Vect2&	operator-=(const Vect2& rhs) noexcept;
	constexpr Vect2		operator-(const Vect2& rhs) const noexcept;
	constexpr Vect2&	operator*=(T rhs) noexcept;
	constexpr Vect2		operator*(T rhs) const noexcept;
	constexpr Vect2&	operator/=(T rhs) noexcept;
	constexpr Vect2		operator/(T rhs) const noexcept;

	/* BOOLEAN COMPARISON ====================================================== */

	constexpr bool	operator==(const Vect2& rhs) const noexcept;

}; // struct Vect2

template <typename T>
struct Vect3 {

	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	union {
		struct { T x, y, z; };
		struct { T r, g, b; };
		struct { T u, v, w; };
		Vect2<T> xy;
		Vect2<T> uv;
		T elem[3];
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	constexpr Vect3(): x(T(0)), y(T(0)), z(T(0)) {}
	constexpr Vect3(T value): x(value), y(value), z(value) {}
	constexpr Vect3(T new_x, T new_y, T new_z):
		x(new_x), y(new_y), z(new_z) {}
    constexpr Vect3(const Vect2<T>& vec, T val):
        x(vec.x), y(vec.y), z(val) {}
    constexpr Vect3(T val, const Vect2<T>& vec):
        x(val), y(vec.x), z(vec.y) {}

	constexpr Vect3(const Vect3& other) = default;
	constexpr Vect3(Vect3&& other) = default;
	constexpr Vect3& operator=(const Vect3& rhs) = default;
	constexpr Vect3& operator=(Vect3&& rhs) = default;
    constexpr Vect3& operator=(T rhs) noexcept { x = rhs; y = rhs; z = rhs; return *this; }
	~Vect3() = default;

    template <typename U>
    explicit constexpr operator Vect3<U>() const noexcept {
        return Vect3<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
    }

	/* ACCESSORS =============================================================== */

	constexpr const T&	operator[](std::size_t index) const;
	constexpr T&		operator[](std::size_t index);

	/* OPERATORS =============================================================== */

	constexpr bool		operator!() const noexcept;
	constexpr Vect3		operator-() const noexcept;
	constexpr Vect3&	operator+=(const Vect3& rhs) noexcept;
	constexpr Vect3		operator+(const Vect3& rhs) const noexcept;
	constexpr Vect3&	operator-=(const Vect3& rhs) noexcept;
	constexpr Vect3		operator-(const Vect3& rhs) const noexcept;
	constexpr Vect3&	operator*=(T rhs) noexcept;
	constexpr Vect3		operator*(T rhs) const noexcept;
	constexpr Vect3&	operator/=(T rhs) noexcept;
	constexpr Vect3		operator/(T rhs) const noexcept;
	constexpr Vect3&	operator*=(const Vect3& rhs) noexcept;
	constexpr Vect3		operator*(const Vect3& rhs) const noexcept;
	constexpr Vect3&	operator/=(const Vect3& rhs) noexcept;
	constexpr Vect3		operator/(const Vect3& rhs) const noexcept;

    constexpr uint32_t  toRGBA() const noexcept;

	/* BOOLEAN COMPARISON ====================================================== */

	constexpr bool	operator==(const Vect3& rhs) const noexcept;

}; // struct Vect3

template <typename T>
struct Vect4 {
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	union {
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		Vect3<T> xyz;
		Vect3<T> rgb;
		Vect2<T> xy;
		Vect2<T> rg;
		T elem[4];
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	constexpr Vect4(): x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}
	constexpr Vect4(T value): x(value), y(value), z(value), w(value) {}
	constexpr Vect4(T new_x, T new_y, T new_z, T new_w):
		x(new_x), y(new_y), z(new_z), w(new_w) {}
    constexpr Vect4(const Vect3<T>& vec, T val):
        x(vec.x), y(vec.y), z(vec.z), w(val) {}
    constexpr Vect4(T val, const Vect3<T>& vec):
        x(val), y(vec.x), z(vec.y), w(vec.z) {}
    constexpr Vect4(const Vect2<T>& first, const Vect2<T>& sec):
        x(first.x), y(first.y), z(sec.x), w(sec.y) {}
    constexpr Vect4(const Vect2<T>& vec, T blue, T alpha):
        x(vec.x), y(vec.y), z(blue), w(alpha) {}
    constexpr Vect4(T red, const Vect2<T>& vec, T alpha):
        x(red), y(vec.x), z(vec.y), w(alpha) {}
    constexpr Vect4(T red, T green, const Vect2<T>& vec):
        x(red), y(green), z(vec.x), w(vec.y) {}

	constexpr Vect4(const Vect4& other) = default;
	constexpr Vect4(Vect4&& other) = default;
	constexpr Vect4& operator=(const Vect4& rhs) = default;
	constexpr Vect4& operator=(Vect4&& rhs) = default;
    constexpr Vect4& operator=(T rhs) noexcept { x = rhs; y = rhs; z = rhs; w = rhs; return *this; }
	~Vect4() = default;

    template <typename U>
    explicit constexpr operator Vect4<U>() const noexcept {
        return Vect4<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w));
    }

	/* ACCESSORS =============================================================== */

	constexpr const T&	operator[](std::size_t index) const;
	constexpr T&		operator[](std::size_t index);

	/* OPERATORS =============================================================== */

	constexpr bool		operator!() const noexcept;
	constexpr Vect4		operator-() const noexcept;
	constexpr Vect4&	operator+=(const Vect4& rhs) noexcept;
	constexpr Vect4		operator+(const Vect4& rhs) const noexcept;
	constexpr Vect4&	operator-=(const Vect4& rhs) noexcept;
	constexpr Vect4		operator-(const Vect4& rhs) const noexcept;
	constexpr Vect4&	operator*=(T rhs) noexcept;
	constexpr Vect4		operator*(T rhs) const noexcept;
	constexpr Vect4&	operator/=(T rhs) noexcept;
	constexpr Vect4		operator/(T rhs) const noexcept;
	constexpr Vect4&	operator*=(const Vect4& rhs) noexcept;
	constexpr Vect4		operator*(const Vect4& rhs) const noexcept;
	constexpr Vect4&	operator/=(const Vect4& rhs) noexcept;
	constexpr Vect4		operator/(const Vect4& rhs) const noexcept;

    constexpr uint32_t  toRGBA() const noexcept;

	/* BOOLEAN COMPARISON ====================================================== */

	constexpr bool	operator==(const Vect4& rhs) const noexcept;

}; // struct Vect4

/* ========================================================================== */
/*                                                                            */
/*                                    VECT2                                   */
/*                                                                            */
/* ========================================================================== */

/* ACCESSORS =============================================================== */

template <typename T>
constexpr
T&	Vect2<T>::operator[](std::size_t index) {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		default:
			throw std::out_of_range("Matrix index out of range");
	}
}

template <typename T>
constexpr
const T&	Vect2<T>::operator[](std::size_t index) const {
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

template <typename T>
constexpr
bool	Vect2<T>::operator!() const noexcept {
	return !x && !y;
}

template <typename T>
constexpr
Vect2<T>	Vect2<T>::operator-() const noexcept {
	return Vect2<T>{ -x, -y };
}

template <typename T>
constexpr
Vect2<T>&	Vect2<T>::operator+=(const Vect2<T>& rhs) noexcept {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

template <typename T>
constexpr
Vect2<T>	Vect2<T>::operator+(const Vect2<T>& rhs) const noexcept {
	Vect2<T>	res(*this);
	return res.operator+=(rhs);
}

template <typename T>
constexpr
Vect2<T>&	Vect2<T>::operator-=(const Vect2<T>& rhs) noexcept {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

template <typename T>
constexpr
Vect2<T>	Vect2<T>::operator-(const Vect2<T>& rhs) const noexcept {
	Vect2<T>	res(*this);
	return res.operator-=(rhs);
}

template <typename T>
constexpr
Vect2<T>&	Vect2<T>::operator*=(T rhs) noexcept {
	x *= rhs;
	y *= rhs;
	return *this;
}

template <typename T>
constexpr
Vect2<T>	Vect2<T>::operator*(T rhs) const noexcept {
	Vect2<T>	res(*this);
	return res.operator*=(rhs);
}

template <typename T>
constexpr
Vect2<T>&	Vect2<T>::operator/=(T rhs) noexcept {
	x /= rhs;
	y /= rhs;
	return *this;
}

template <typename T>
constexpr
Vect2<T>	Vect2<T>::operator/(T rhs) const noexcept {
	Vect2<T>	res(*this);
	return res.operator/=(rhs);
}

/* BOOLEAN COMPARISON ====================================================== */

template <typename T>
constexpr
bool	Vect2<T>::operator==(const Vect2<T>& rhs) const noexcept {
	return x == rhs.x && y == rhs.y;
}

/* ========================================================================== */

/**
 * @brief Returns the dot product of the vector with another vector
 */
template <typename T>
inline T	dot(const Vect2<T>& lhs, const Vect2<T>& rhs) noexcept {
	return std::fma(lhs.x, rhs.x, lhs.y * rhs.y);
}

/**
 * @brief Returns the norm of the vector
*/
template <typename T>
inline T	norm(const Vect2<T>& vec) noexcept {
	return std::sqrt(dot(vec, vec));
}

/**
 * @brief Returns a normalized vector
*/
template <typename T>
inline Vect2<T>	normalize(const Vect2<T>& vec) noexcept {
	T	n = norm(vec);
	return Vect2<T>{ vec.x / n, vec.y / n };
}

/* ========================================================================== */
/*                                                                            */
/*                                    VECT3                                   */
/*                                                                            */
/* ========================================================================== */

/* ACCESSORS =============================================================== */

template <typename T>
constexpr
const T& Vect3<T>::operator[](std::size_t index) const {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range("Vect index out of range");
	}
}

template <typename T>
constexpr
T& Vect3<T>::operator[](std::size_t index) {
	switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range("Vect index out of range");
	}
}

/* OPERATORS =============================================================== */

template <typename T>
constexpr
bool Vect3<T>::operator!() const noexcept {
	return !x && !y && !z;
}

template <typename T>
constexpr
Vect3<T>	Vect3<T>::operator-() const noexcept {
	return Vect3<T>(-x, -y, -z);
}

template <typename T>
constexpr
Vect3<T>&	Vect3<T>::operator+=(const Vect3<T>& rhs) noexcept {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

template <typename T>
constexpr
Vect3<T>	Vect3<T>::operator+(const Vect3<T>& rhs) const noexcept {
	Vect3<T>	res(*this);
	return res.operator+=(rhs);
}

template <typename T>
constexpr
Vect3<T>&	Vect3<T>::operator-=(const Vect3<T>& rhs) noexcept {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

template <typename T>
constexpr
Vect3<T>	Vect3<T>::operator-(const Vect3<T>& rhs) const noexcept {
	Vect3<T>	res(*this);
	return res.operator-=(rhs);
}

template <typename T>
constexpr
Vect3<T>&	Vect3<T>::operator*=(T rhs) noexcept {
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

template <typename T>
constexpr
Vect3<T>	Vect3<T>::operator*(T rhs) const noexcept {
	Vect3<T>	res(*this);
	return res.operator*=(rhs);
}

template <typename T>
constexpr
Vect3<T>&	Vect3<T>::operator/=(T rhs) noexcept {
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

template <typename T>
constexpr
Vect3<T>	Vect3<T>::operator/(T rhs) const noexcept {
	Vect3<T>	res(*this);
	return res.operator/=(rhs);
}

template <typename T>
constexpr
Vect3<T>& Vect3<T>::operator*=(const Vect3<T>& rhs) noexcept {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}

template <typename T>
constexpr
Vect3<T> Vect3<T>::operator*(const Vect3<T>& rhs) const noexcept {
	Vect3<T>	res(*this);
	return res.operator*=(rhs);
}

template <typename T>
constexpr
Vect3<T>& Vect3<T>::operator/=(const Vect3<T>& rhs) noexcept {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	return *this;
}

template <typename T>
constexpr
Vect3<T> Vect3<T>::operator/(const Vect3<T>& rhs) const noexcept {
	Vect3<T>	res(*this);
	return res.operator/=(rhs);
}

template <typename T>
constexpr
uint32_t Vect3<T>::toRGBA() const noexcept {
    Vect3<T> color(*this * 255.0f);
    return ((uint32_t)color.x << 24) | ((uint32_t)color.y << 16) | ((uint32_t)color.z << 8) | 255;
}

/* BOOLEAN COMPARISON ====================================================== */

template <typename T>
constexpr
bool	Vect3<T>::operator==(const Vect3<T>& rhs) const noexcept {
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

/* ========================================================================== */

/**
 * @brief Returns the dot product of the vector with another vector
 */
template <typename T>
inline T	dot(const Vect3<T>& lhs, const Vect3<T>& rhs) noexcept {
	return std::fma(lhs.x, rhs.x, std::fma(lhs.y, rhs.y, lhs.z * rhs.z));
}

/**
 * @brief Returns the norm of the vector
 */
template <typename T>
inline T	norm(const Vect3<T>& vec) noexcept {
	return std::sqrt(dot(vec, vec));
}

/**
 * @brief Returns a normalized vector
 */
template <typename T>
inline Vect3<T>	normalize(const Vect3<T>& vec) noexcept {
	T	n = norm(vec);
	return Vect3<T>{ vec.x / n, vec.y / n, vec.z / n };
}

/**
 * @brief Returns the cross product of the vector with another vector
 */
template <typename T>
inline Vect3<T>	cross(const Vect3<T>& lhs, const Vect3<T>& rhs) noexcept {
	return Vect3<T>(
		std::fma(lhs.y, rhs.z, -lhs.z * rhs.y),
		std::fma(lhs.z, rhs.x, -lhs.x * rhs.z),
		std::fma(lhs.x, rhs.y, -lhs.y * rhs.x)
	);
}

/**
 * @brief Special fma for Vect3<T> and T: a * b + c.
 *
 * @param a		The vector to multiply.
 * @param b		The scalar to multiply.
 * @param c		The vector to add.
*/
template <typename T>
inline Vect3<T>	fma(const Vect3<T>& a, T b, const Vect3<T>& c) noexcept {
	return Vect3<T>(
		std::fma(a.x, b, c.x),
		std::fma(a.y, b, c.y),
		std::fma(a.z, b, c.z)
	);
}

/**
 * @brief Special lerp for Vect3<T> and T: a * (1 - t) + b * t.
 *
 * @param a		The first vector.
 * @param b		The second vector.
 * @param t		The interpolation value.
*/
template <typename T>
inline Vect3<T>    lerp(const Vect3<T>& a, const Vect3<T>& b, const T t) noexcept {
    return fma(b - a, t, a);
}

/* ========================================================================== */
/*                                                                            */
/*                                    VECT4                                   */
/*                                                                            */
/* ========================================================================== */

/* ACCESSORS =============================================================== */

template <typename T>
constexpr
const T& Vect4<T>::operator[](std::size_t index) const {
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

template <typename T>
constexpr
T& Vect4<T>::operator[](std::size_t index) {
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

template <typename T>
constexpr
bool Vect4<T>::operator!() const noexcept {
	return !x && !y && !z && !w;
}

template <typename T>
constexpr
Vect4<T>	Vect4<T>::operator-() const noexcept {
	return Vect4<T>(-x, -y, -z, -w);
}

template <typename T>
constexpr
Vect4<T>&	Vect4<T>::operator+=(const Vect4<T>& rhs) noexcept {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

template <typename T>
constexpr
Vect4<T>	Vect4<T>::operator+(const Vect4<T>& rhs) const noexcept {
	Vect4<T>	res(*this);
	return res.operator+=(rhs);
}

template <typename T>
constexpr
Vect4<T>&	Vect4<T>::operator-=(const Vect4<T>& rhs) noexcept {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

template <typename T>
constexpr
Vect4<T>	Vect4<T>::operator-(const Vect4<T>& rhs) const noexcept {
	Vect4<T>	res(*this);
	return res.operator-=(rhs);
}

template <typename T>
constexpr
Vect4<T>&	Vect4<T>::operator*=(T rhs) noexcept {
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
	return *this;
}

template <typename T>
constexpr
Vect4<T>	Vect4<T>::operator*(T rhs) const noexcept {
	Vect4<T>	res(*this);
	return res.operator*=(rhs);
}

template <typename T>
constexpr
Vect4<T>&	Vect4<T>::operator/=(T rhs) noexcept {
	x /= rhs;
	y /= rhs;
	z /= rhs;
	w /= rhs;
	return *this;
}

template <typename T>
constexpr
Vect4<T>	Vect4<T>::operator/(T rhs) const noexcept {
	Vect4<T>	res(*this);
	return res.operator/=(rhs);
}

template <typename T>
constexpr
Vect4<T>& Vect4<T>::operator*=(const Vect4<T>& rhs) noexcept {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
    w *= rhs.w;
	return *this;
}

template <typename T>
constexpr
Vect4<T> Vect4<T>::operator*(const Vect4<T>& rhs) const noexcept {
	Vect4<T>	res(*this);
	return res.operator*=(rhs);
}

template <typename T>
constexpr
Vect4<T>& Vect4<T>::operator/=(const Vect4<T>& rhs) noexcept {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
    w /= rhs.w;
	return *this;
}

template <typename T>
constexpr
Vect4<T> Vect4<T>::operator/(const Vect4<T>& rhs) const noexcept {
	Vect4<T>	res(*this);
	return res.operator/=(rhs);
}

template <typename T>
constexpr
uint32_t Vect4<T>::toRGBA() const noexcept {
    Vect4<T> color(*this * 255.0f);
    return ((uint32_t)color.x << 24) | ((uint32_t)color.y << 16) | ((uint32_t)color.z << 8) | (uint32_t)color.w;
}

/* BOOLEAN COMPARISON ====================================================== */

template <typename T>
constexpr
bool	Vect4<T>::operator==(const Vect4<T>& rhs) const noexcept {
	return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

/* ========================================================================== */

/**
 * @brief Returns the dot product of the vector with another vector
 */
template <typename T>
inline T	dot(const Vect4<T>& lhs, const Vect4<T>& rhs) noexcept {
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
template <typename T>
inline T	norm(const Vect4<T>& vec) noexcept {
	return std::sqrt(dot(vec, vec));
}

/**
 * @brief Returns a normalized vector
 */
template <typename T>
inline Vect4<T>	normalize(const Vect4<T>& vec) noexcept {
	T	n = norm(vec);
	return Vect4<T>{ vec.x / n, vec.y / n, vec.z / n, vec.w / n };
}

/**
 * @brief Special fma for Vect4<T> and T: a * b + c.
 *
 * @param a		The vector to multiply.
 * @param b		The scalar to multiply.
 * @param c		The vector to add.
*/
template <typename T>
inline Vect4<T>	fma(const Vect4<T>& a, T b, const Vect4<T>& c) noexcept {
	return Vect4<T>(
		std::fma(a.x, b, c.x),
		std::fma(a.y, b, c.y),
		std::fma(a.z, b, c.z),
		std::fma(a.w, b, c.w)
	);
}

/* ========================================================================== */
/*                                  TYPEDEFS                                  */
/* ========================================================================== */

using vec2 = math::Vect2<float>;
using uvec2 = math::Vect2<unsigned int>;
using ivec2 = math::Vect2<int>;
using dvec2 = math::Vect2<double>;

using vec3 = math::Vect3<float>;
using uvec3 = math::Vect3<unsigned int>;
using ivec3 = math::Vect3<int>;
using dvec3 = math::Vect3<double>;

using vec4 = math::Vect4<float>;
using uvec4 = math::Vect4<unsigned int>;
using ivec4 = math::Vect4<int>;
using dvec4 = math::Vect4<double>;

} // namespace math

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

/* OPERATIONS =============================================================== */

template <class Vector>
constexpr
Vector	operator*(float lhs, const Vector& rhs) noexcept {
	return rhs.operator*(lhs);
}

template <class Vector>
constexpr
Vector	operator/(float lhs, const Vector& rhs) noexcept {
	return rhs.operator/(lhs);
}

template <class Vector>
constexpr
Vector	abs(const Vector& vect) noexcept {
	Vector	copy;
	int i = 0;
	for (float elem: vect.elem)
		copy.elem[i++] = std::abs(elem);
	return copy;
}

/* HASH FUNCTIONS =========================================================== */

template<>
struct std::hash<math::Vect3<float>> {
	inline std::size_t	operator()(const math::Vect3<float>& vect) const {
		return (
			std::hash<float>()(vect.x) ^
			std::hash<float>()(vect.y) ^
			std::hash<float>()(vect.z)
		);
	}
};

template<>
struct std::hash<math::Vect2<float>> {
	inline std::size_t	operator()(const math::Vect2<float>& vect) const {
		return (
			std::hash<float>()(vect.x) ^
			std::hash<float>()(vect.y)
		);
	}
};

/* IOSTREAM ================================================================= */

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const math::Vect3<T>& vect) {
	os << "{x:" << vect.x << ", y:" << vect.y << ", z:" << vect.z << "}";
	return os;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const math::Vect2<T>& vect) {
	os << "{x:" << vect.x << ", y:" << vect.y << "}";
	return os;
}