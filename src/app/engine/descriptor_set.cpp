/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:33:52 by etran             #+#    #+#             */
/*   Updated: 2023/08/22 21:42:09 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_set.h"
#include "device.h"

#include "utils.h"

namespace scop::graphics {

uint32_t	DescriptorSet::_descriptor_count = 0;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

DescriptorSet::DescriptorSet(): _index(_descriptor_count++) {}

/* ========================================================================== */

void	DescriptorSet::destroy(Device& device) {
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

const DescriptorSet::DescriptorSizes&	DescriptorSet::getPoolSizes() const noexcept {
	return _writes_sizes;
}

uint32_t	DescriptorSet::getSetIndex() const noexcept {
	return _index;
}

} // namespace scop::graphics