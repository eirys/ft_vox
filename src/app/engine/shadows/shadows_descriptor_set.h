/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_descriptor_set.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 11:34:45 by etran             #+#    #+#             */
/*   Updated: 2023/09/01 15:30:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "descriptor_set.h"

namespace scop::graphics {

class Buffer;
class TextureHandler;

class ShadowsDescriptorSet final: public DescriptorSet {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = DescriptorSet;
	using TextureHandlerPtr = std::shared_ptr<TextureHandler>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ShadowsDescriptorSet() = default;
	~ShadowsDescriptorSet() = default;

	ShadowsDescriptorSet(ShadowsDescriptorSet&& other) = delete;
	ShadowsDescriptorSet(const ShadowsDescriptorSet& other) = delete;
	ShadowsDescriptorSet& operator=(ShadowsDescriptorSet&& rhs) = delete;
	ShadowsDescriptorSet& operator=(const ShadowsDescriptorSet& rhs) = delete;

	/* ========================================================================= */

	using super::destroy;
	using super::setDescriptors;

	void	init(Device& device) override;

	void	plug(
		Device& device,
		Buffer& buffer,
		TextureHandlerPtr heightmap);

	/* ========================================================================= */

	using super::getLayout;
	using super::getPoolSizes;
	using super::getSet;
	using super::getSetIndex;

}; // class ShadowsDescriptorSet

} // namespace scop::graphics