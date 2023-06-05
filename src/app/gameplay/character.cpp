/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   character.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:57:24 by etran             #+#    #+#             */
/*   Updated: 2023/06/05 23:40:27 by etran            ###   ########.fr       */
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
	position(pos),
	eye_dir(dir),
	cam_speed(speed) {
		camera.yaw = scop::math::dregrees(std::atan2(dir.z, dir.x));
		camera.pitch = scop::math::dregrees(std::asin(dir.y));
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
		scop::cross(eye_dir, y_axis)
	);
	position = scop::fma(
		eye_dir,
		input.z,
		scop::fma(
			y_axis,
			input.y,
			scop::fma(side, input.x, position)
		)
	);
}

/**
 * @brief Updates the eye direction of the character, using yaw and pitch.
 *
 * @param yaw	The added yaw, in degrees.
 * @param pitch	The added pitch, in degrees.
*/
void	Character::updateEyeDir(float yaw, float pitch) noexcept {
	camera.yaw = std::fma(yaw, cam_speed, camera.yaw);

	// Clamp to avoid camera flipping.
	camera.pitch = std::clamp(
		std::fma(pitch, cam_speed, camera.pitch),
		-89.0f,
		89.0f
	);
	eye_dir = scop::normalize(camera.toVector());
}

/**
 * @brief Resets the eye direction of the character to dir.
 *
 * @param dir	The new direction vector.
*/
void	Character::resetEyeDir(const scop::Vect3& dir) noexcept {
	eye_dir = dir;
	camera.yaw = scop::math::dregrees(std::atan2(dir.z, dir.x));
	camera.pitch = scop::math::dregrees(std::asin(dir.y));
}

void	Character::resetPositionX(float x) noexcept {
	position.x = x;
}

void	Character::resetPositionY(float y) noexcept {
	position.y = y;
}

void	Character::resetPositionZ(float z) noexcept {
	position.z = z;
}

/* ========================================================================== */

/**
 * @brief Returns the position of the character.
*/
const scop::Vect3&	Character::getPosition() const noexcept {
	return position;
}

/**
 * @brief Returns the eye direction of the character.
*/
const scop::Vect3&	Character::getEyeDir() const noexcept {
	return eye_dir;
}

/**
 * @brief Returns the camera of the character.
 * @note The camera is a struct containing the yaw and pitch of the character.
*/
const Character::Camera&	Character::getCamera() const noexcept {
	return camera;
}

} // namespace vox