/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_target.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 16:21:56 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 21:55:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "target.h"

namespace scop::graphics {

class SceneTarget final: public Target {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = Target;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	SceneTarget() = default;
	~SceneTarget() = default;

	SceneTarget(SceneTarget&& other) = delete;
	SceneTarget(const SceneTarget& other) = delete;
	SceneTarget& operator=(SceneTarget&& other) = delete;
	SceneTarget& operator=(const SceneTarget& other) = delete;

	/* ========================================================================= */

	using super::destroy;

	void				init(
		Device& device,
		const super::TargetInfo& info) override;
	void				update(
		Device& device,
		const super::TargetInfo& info) override;

	/* ========================================================================= */

	using super::getFrameBuffers;

}; // class SceneTarget

} // namespace scop::graphics