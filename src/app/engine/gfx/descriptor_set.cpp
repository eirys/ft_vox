/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:33:52 by etran             #+#    #+#             */
/*   Updated: 2024/01/05 12:45:16 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_set.h"
#include "device.h"
#include "descriptor_pool.h"

#include <cassert> // assert

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

DescriptorSet::DescriptorSet(const uint32_t index): _index(index) {}

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

VkDescriptorSetLayoutBinding	DescriptorSet::createLayoutBinding(
	DescriptorType type,
	VkShaderStageFlags shader_stage,
	uint32_t binding_index,
	uint32_t count
) {
	VkDescriptorSetLayoutBinding	layout_binding = {};
	layout_binding.binding = binding_index;
	layout_binding.descriptorCount = count;
	layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
	layout_binding.stageFlags = shader_stage;

	switch (type) {
		case DescriptorType::UNIFORM_BUFFER:
			layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			++_writes_sizes.uniform_buffer;
			break;

		case DescriptorType::COMBINED_IMAGE_SAMPLER:
			layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			++_writes_sizes.combined_image_sampler;
			break;

		case DescriptorType::STORAGE_BUFFER:
			layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			++_writes_sizes.storage_buffer;
			break;

		case DescriptorType::STORAGE_IMAGE:
			layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			++_writes_sizes.storage_image;
			break;

		case DescriptorType::SAMPLED_IMAGE:
			layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			++_writes_sizes.sampled_image;
			break;

		default:
			assert(false && "Descriptor type not found");
	}
	return layout_binding;
}

VkWriteDescriptorSet	DescriptorSet::createWriteDescriptorSet(
	DescriptorType type,
	void* descriptor_info,
	uint32_t binding_index,
	uint32_t count
) const {
	VkWriteDescriptorSet	write_descriptor_set{};
	write_descriptor_set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write_descriptor_set.dstSet = _set;
	write_descriptor_set.dstBinding = binding_index;
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
			break;

		case DescriptorType::COMBINED_IMAGE_SAMPLER:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write_descriptor_set.pBufferInfo = nullptr;
			write_descriptor_set.pImageInfo = reinterpret_cast<VkDescriptorImageInfo*>(descriptor_info);
			break;

		case DescriptorType::STORAGE_BUFFER:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			write_descriptor_set.pBufferInfo = reinterpret_cast<VkDescriptorBufferInfo*>(descriptor_info);
			write_descriptor_set.pImageInfo = nullptr;
			break;

		case DescriptorType::STORAGE_IMAGE:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			write_descriptor_set.pBufferInfo = nullptr;
			write_descriptor_set.pImageInfo = reinterpret_cast<VkDescriptorImageInfo*>(descriptor_info);
			break;

		case DescriptorType::SAMPLED_IMAGE:
			write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			write_descriptor_set.pBufferInfo = nullptr;
			write_descriptor_set.pImageInfo = reinterpret_cast<VkDescriptorImageInfo*>(descriptor_info);
			break;

		default:
			assert(false && "Descriptor type not found");
	}
}

} // namespace scop::gfx