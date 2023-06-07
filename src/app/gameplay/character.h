/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   character.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 09:55:33 by etran             #+#    #+#             */
/*   Updated: 2023/06/07 14:30:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "math.h"
# include "vector.h"

namespace vox {

class Character {
public:
	/* ========================================================================= */
	/*                                HELPER CLASS                               */
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
		float speed
	);

	virtual ~Character() = default;

	Character(Character&& other) = delete;
	Character() = delete;
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

	scop::Vect3			position;
	scop::Vect3			eye_dir;
	Character::Camera	camera;
	const float			cam_speed;

}; // class Character

} // namespace vox