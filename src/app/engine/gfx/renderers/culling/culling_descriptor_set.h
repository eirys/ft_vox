/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_descriptor_set.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 11:52:18 by etran             #+#    #+#             */
/*   Updated: 2023/12/23 21:18:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

# include "descriptor_set.h"

namespace scop {
class InputHandler;
}
namespace scop::gfx {

class Buffer;

class CullingDescriptorSet final: public DescriptorSet {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = DescriptorSet;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	CullingDescriptorSet() = default;
	~CullingDescriptorSet() = default;

	CullingDescriptorSet(CullingDescriptorSet&& other) = delete;
	CullingDescriptorSet(const CullingDescriptorSet& other) = delete;
	CullingDescriptorSet& operator=(CullingDescriptorSet&& rhs) = delete;
	CullingDescriptorSet& operator=(const CullingDescriptorSet& rhs) = delete;

	/* ========================================================================= */

	using super::destroy;
	using super::setDescriptors;

	void	init(scop::core::Device& device) override;
	void	plug(
		scop::core::Device& device,
		const scop::InputHandler& input);

	/* ========================================================================= */

	using super::getLayout;
	using super::getPoolSizes;
	using super::getSet;
	using super::getSetIndex;

}; // class CullingDescriptorSet

} // namespace scop::gfx
