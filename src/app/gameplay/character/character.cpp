/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   character.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:57:24 by etran             #+#    #+#             */
/*   Updated: 2023/09/22 16:58:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "character.h"

namespace vox {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Character::Character(
	const scop::Vect3& pos,
	const scop::Vect3& dir,
	float speed
):
	_position(pos),
	_eye_dir(dir),
	_cam_speed(speed) {
		_camera.yaw = scop::math::dregrees(std::atan2(dir.z, dir.x));
		_camera.pitch = scop::math::dregrees(std::asin(dir.y));
	}

/* ========================================================================== */

/**
 * @brief Updates the position of the character.
 *
 * @param input	The input vector, containing the movement on each axis.
*/
void	Character::move(const scop::Vect3& input) noexcept {
	static const scop::Vect3	y_axis = scop::Vect3(0.0f, 1.0f, 0.0f);
	const scop::Vect3	side = scop::normalize(
		scop::cross(_eye_dir, y_axis)
	);
	_position = scop::fma(
		_eye_dir,
		input.z,
		scop::fma(y_axis, input.y, scop::fma(side, input.x, _position)));
}

/**
 * @brief Updates the eye direction of the character, using yaw and pitch.
 *
 * @param yaw	The added yaw, in degrees.
 * @param pitch	The added pitch, in degrees.
*/
void	Character::updateEyeDir(float yaw, float pitch) noexcept {
	_camera.yaw = std::fma(yaw, _cam_speed, _camera.yaw);

	// Clamp to avoid camera flipping.
	_camera.pitch = std::clamp(
		std::fma(pitch, _cam_speed, _camera.pitch), -89.0f, 89.0f);
	_eye_dir = scop::normalize(_camera.toVector());
}

/**
 * @brief Resets the eye direction of the character to dir.
 *
 * @param dir	The new direction vector.
*/
void	Character::resetEyeDir(const scop::Vect3& dir) noexcept {
	_eye_dir = dir;
	_camera.yaw = scop::math::dregrees(std::atan2(dir.z, dir.x));
	_camera.pitch = scop::math::dregrees(std::asin(dir.y));
}

void	Character::resetPositionX(float x) noexcept {
	_position.x = x;
}

void	Character::resetPositionY(float y) noexcept {
	_position.y = y;
}

void	Character::resetPositionZ(float z) noexcept {
	_position.z = z;
}

/* ========================================================================== */

/**
 * @brief Returns the position of the character.
*/
const scop::Vect3&	Character::getPosition() const noexcept {
	return _position;
}

/**
 * @brief Returns the eye direction of the character.
*/
const scop::Vect3&	Character::getEyeDir() const noexcept {
	return _eye_dir;
}

/**
 * @brief Returns the camera of the character.
 * @note The camera is a struct containing the yaw and pitch of the character.
*/
const Character::Camera&	Character::getCamera() const noexcept {
	return _camera;
}

} // namespace vox