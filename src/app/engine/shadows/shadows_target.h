/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_target.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 14:15:12 by etran             #+#    #+#             */
/*   Updated: 2023/08/17 14:15:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "target.h"

namespace scop::graphics {

class ShadowsTarget final: public Target {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = Target;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ShadowsTarget() = default;
	~ShadowsTarget() = default;

	ShadowsTarget(ShadowsTarget&& other) = delete;
	ShadowsTarget(const ShadowsTarget& other) = delete;
	ShadowsTarget& operator=(ShadowsTarget&& other) = delete;
	ShadowsTarget& operator=(const ShadowsTarget& other) = delete;

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

}; // class ShadowsTarget

} // namespace scop::graphics