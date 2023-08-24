/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:56:05 by etran             #+#    #+#             */
/*   Updated: 2023/08/22 21:42:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_pool.h"
#include "device.h"
#include "descriptor_set.h"

#include <array> // std::array
#include <list> // std::list
#include <stdexcept> // std::runtime_error

namespace scop::graphics {

using DescriptorSetPtr = DescriptorPool::DescriptorSetPtr;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Saves a descriptors.
*/
void	DescriptorPool::add(DescriptorSetPtr set) {
	_descriptors.emplace_back(set);
}

/**
 * @brief Descriptor set layout for uniform buffer and combined image sampler.
*/
void	DescriptorPool::init(
	Device& device
) {
	_createDescriptorPool(device);
	_allocateSets(device);
	_createWrites(device);
}

void	DescriptorPool::destroy(Device& device) {
	for (auto& set: _descriptors) {
		set->destroy(device);
	}

	vkDestroyDescriptorPool(device.getLogicalDevice(), _pool, nullptr);
}

/* ========================================================================== */

VkDescriptorPool		DescriptorPool::getPool() const noexcept {
	return _pool;
}

const std::vector<VkDescriptorSetLayout>&	DescriptorPool::getLayouts() const noexcept {
	return _layouts;
}

const std::vector<DescriptorSetPtr>&	DescriptorPool::getDescriptors() const noexcept {
	return _descriptors;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Handler for descriptor sets allocation.
*/
void	DescriptorPool::_createDescriptorPool(
	Device& device
) {
	std::array<VkDescriptorPoolSize, 2>	pool_sizes;

	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[0].descriptorCount = 0;
	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[1].descriptorCount = 0;

	for (const auto& set: _descriptors) {
		const auto& sizes = set->getPoolSizes();
		pool_sizes[0].descriptorCount += sizes.uniform_buffer;
		pool_sizes[1].descriptorCount += sizes.combined_image_sampler;
	}

	VkDescriptorPoolCreateInfo	pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
	pool_info.pPoolSizes = pool_sizes.data();
	pool_info.maxSets = static_cast<uint32_t>(_descriptors.size());

	if (vkCreateDescriptorPool(device.getLogicalDevice(), &pool_info, nullptr, &_pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool");
	}
}

/**
 * @brief Allocate every descriptor sets.
*/
void	DescriptorPool::_allocateSets(
	Device& device
) {
	std::vector<VkDescriptorSet>	descriptor_sets;

	_layouts.reserve(_descriptors.size());
	descriptor_sets.resize(_descriptors.size());

	for (const auto& set: _descriptors) {
		_layouts.emplace_back(set->getLayout());
	}

	VkDescriptorSetAllocateInfo	alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = _pool;
	alloc_info.descriptorSetCount = static_cast<uint32_t>(descriptor_sets.size());
	alloc_info.pSetLayouts = _layouts.data();

	if (vkAllocateDescriptorSets(device.getLogicalDevice(), &alloc_info, descriptor_sets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets");
	}

	for (std::size_t i = 0; i < _descriptors.size(); ++i) {
		_descriptors[i]->_set = descriptor_sets[i];
	}

}

/**
 * @brief Reconstructs descriptor writes array from sets,
 * 		  then updates descriptors, properly plugging cpu/gpu sides
*/
void	DescriptorPool::_createWrites(Device& device) {
	using DescriptorPtr = DescriptorSet::DescriptorPtr;
	using BufferInfo = DescriptorSet::BufferInfo;
	using ImageInfo = DescriptorSet::ImageInfo;

	std::vector<VkWriteDescriptorSet>	writes;
	std::list<VkDescriptorBufferInfo>	buffers;
	std::list<VkDescriptorImageInfo>	images;

	for (const auto& descriptor: _descriptors) {
		for (const auto& info: descriptor->getInfos()) {
			VkWriteDescriptorSet	write{};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet = descriptor->getSet();
			write.dstBinding = info->binding;
			write.dstArrayElement = 0;
			write.descriptorType = info->type;
			write.descriptorCount = 1;

			if (info->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
				std::shared_ptr<BufferInfo> buffer_info = std::dynamic_pointer_cast<BufferInfo>(info);
				VkDescriptorBufferInfo	buffer{};
				buffer.buffer = descriptor->getBuffer().getBuffer();
				buffer.offset = buffer_info->offset;
				buffer.range = buffer_info->range;
				write.pBufferInfo = &buffers.emplace_back(buffer);
			} else {
				std::shared_ptr<ImageInfo> image_info = std::dynamic_pointer_cast<ImageInfo>(info);
				VkDescriptorImageInfo	image{};
				image.imageLayout = image_info->layout;
				image.imageView = image_info->view;
				image.sampler = image_info->sampler;
				write.pImageInfo = &images.emplace_back(image);
			}

			writes.emplace_back(write);
		}
	}

	vkUpdateDescriptorSets(
		device.getLogicalDevice(),
		static_cast<uint32_t>(writes.size()),
		writes.data(),
		0, nullptr);
}

} // namespace scop::graphics