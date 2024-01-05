/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_descriptor_set.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 11:52:18 by etran             #+#    #+#             */
/*   Updated: 2024/01/05 12:45:54 by etran            ###   ########.fr       */
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

	CullingDescriptorSet();

	~CullingDescriptorSet() = default;

	CullingDescriptorSet(CullingDescriptorSet&& other) = delete;
	CullingDescriptorSet(const CullingDescriptorSet& other) = delete;
	CullingDescriptorSet& operator=(CullingDescriptorSet&& rhs) = delete;
	CullingDescriptorSet& operator=(const CullingDescriptorSet& rhs) = delete;

	/* ========================================================================= */

	using super::destroy;
	using super::setDescriptors;

	void	init(scop::core::Device& device) override;
	void	fill(
		scop::core::Device& device,
		const scop::InputHandler& input);

	/* ========================================================================= */

	using super::getLayout;
	using super::getPoolSizes;
	using super::getSet;
	using super::getSetIndex;

}; // class CullingDescriptorSet

} // namespace scop::gfx
