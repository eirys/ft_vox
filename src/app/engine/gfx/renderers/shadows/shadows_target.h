/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_target.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 14:15:12 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:23:09 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "target.h"

namespace scop::gfx {

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
		scop::core::Device& device,
		const TargetInfo& info) override;
	void				update(
		scop::core::Device& device,
		const TargetInfo& info) override;

	/* ========================================================================= */

	using super::getFrameBuffers;

}; // class ShadowsTarget

} // namespace scop::gfx