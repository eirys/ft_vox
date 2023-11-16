/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   character.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:55:33 by etran             #+#    #+#             */
/*   Updated: 2023/11/17 00:01:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "scop_math.h"
# include "vector.h"

# ifdef __LINUX
#  define VOX_CAMERA_SPEED		0.15f
# else
#  define VOX_CAMERA_SPEED		0.05f
# endif
# define VOX_DEFAULT_EYE_DIR	scop::Vect3(0.0f, 0.0f, 1.0f)

namespace vox {

class Character {
public:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	/**
	 * @brief Contains the camera's yaw and pitch, in degrees.
	 * Used to compute the eye direction.
	*/
	struct Camera {
		float	yaw;
		float	pitch;

		scop::Vect3	toVector() const noexcept {
			const float cos_pitch = std::cos(scop::math::radians(pitch));
			return scop::Vect3(
				std::cos(scop::math::radians(yaw)) * cos_pitch,
				std::sin(scop::math::radians(pitch)),
				std::sin(scop::math::radians(yaw)) * cos_pitch
			);
		}
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Character(
		const scop::Vect3& position,
		const scop::Vect3& eye_dir,
		float speed);

	Character() = default;
	Character(Character&& other) = default;
	Character& operator=(Character&& rhs) = default;
	virtual ~Character() = default;

	Character(const Character& other) = delete;
	Character& operator=(const Character& rhs) = delete;

	/* ========================================================================= */

	virtual void		move(const scop::Vect3& input) noexcept;
	virtual void		updateEyeDir(float yaw, float pitch) noexcept;

	/* ========================================================================= */

	virtual void		resetEyeDir(const scop::Vect3& direction) noexcept;
	virtual void		resetPositionX(float x) noexcept;
	virtual void		resetPositionY(float y) noexcept;
	virtual void		resetPositionZ(float z) noexcept;

	/* ========================================================================= */

	const scop::Vect3&	getPosition() const noexcept;
	const scop::Vect3&	getEyeDir() const noexcept;
	const Camera&		getCamera() const noexcept;

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	scop::Vect3			_position = {0.0f, 0.0f, 0.0f};
	scop::Vect3			_eye_dir = {0.0f, 0.0f, 1.0f};
	Character::Camera	_camera;
	float				_cam_speed = VOX_CAMERA_SPEED;

}; // class Character

} // namespace vox