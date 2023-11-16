/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:33:52 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 22:41:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_set.h"
#include "device.h"

namespace scop::gfx {

uint32_t	DescriptorSet::_descriptor_count = 0;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

DescriptorSet::DescriptorSet(): _index(_descriptor_count++) {}

/* ========================================================================== */

void	DescriptorSet::destroy(scop::core::Device& device) {
	vkDestroyDescriptorSetLayout(
		device.getLogicalDevice(),
		_layout,
		nullptr);
}

void	DescriptorSet::setDescriptors(VkDescriptorSet set) noexcept {
	_set = set;
}

/* ========================================================================== */

VkDescriptorSetLayout	DescriptorSet::getLayout() const noexcept {
	return _layout;
}

VkDescriptorSet	DescriptorSet::getSet() const noexcept {
	return _set;
}

const DescriptorSizes&	DescriptorSet::getPoolSizes() const noexcept {
	return _writes_sizes;
}

uint32_t	DescriptorSet::getSetIndex() const noexcept {
	return _index;
}

} // namespace scop::gfx