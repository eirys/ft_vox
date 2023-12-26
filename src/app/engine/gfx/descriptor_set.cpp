/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:33:52 by etran             #+#    #+#             */
/*   Updated: 2023/12/24 15:52:32 by etran            ###   ########.fr       */
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

/* ========================================================================== */
/*                                  PROTECTED                                 */
/* ========================================================================== */

VkWriteDescriptorSet	DescriptorSet::createWriteDescriptorSet(
	DescriptorType type,
	void* descriptor_info,
	uint32_t binding,
	uint32_t count
) const {
	VkWriteDescriptorSet	write_descriptor_set = {};
	write_descriptor_set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write_descriptor_set.dstSet = _set;
	write_descriptor_set.dstBinding = binding;
	write_descriptor_set.dstArrayElement = 0;
	write_descriptor_set.descriptorCount = count;
	write_descriptor_set.pTexelBufferView = nullptr;

	_fillDescriptorType(type, descriptor_info, write_descriptor_set);

	return write_descriptor_set;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	DescriptorSet::_fillDescriptorType(
	DescriptorType type,
	void* descriptor_info,
	VkWriteDescriptorSet& write_descriptor_set
) const {
	switch (type) {
		case DescriptorType::UNIFORM_BUFFER:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write_descriptor_set.pBufferInfo = reinterpret_cast<VkDescriptorBufferInfo*>(descriptor_info);
			write_descriptor_set.pImageInfo = nullptr;

		case DescriptorType::COMBINED_IMAGE_SAMPLER:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write_descriptor_set.pBufferInfo = nullptr;
			write_descriptor_set.pImageInfo = reinterpret_cast<VkDescriptorImageInfo*>(descriptor_info);

		case DescriptorType::STORAGE_BUFFER:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			write_descriptor_set.pBufferInfo = reinterpret_cast<VkDescriptorBufferInfo*>(descriptor_info);
			write_descriptor_set.pImageInfo = nullptr;

		case DescriptorType::STORAGE_IMAGE:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			write_descriptor_set.pBufferInfo = nullptr;
			write_descriptor_set.pImageInfo = reinterpret_cast<VkDescriptorImageInfo*>(descriptor_info);

		case DescriptorType::SAMPLED_IMAGE:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			write_descriptor_set.pBufferInfo = nullptr;
			write_descriptor_set.pImageInfo = reinterpret_cast<VkDescriptorImageInfo*>(descriptor_info);

		default:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}
}

} // namespace scop::gfx