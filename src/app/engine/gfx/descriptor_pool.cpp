/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:56:05 by etran             #+#    #+#             */
/*   Updated: 2024/01/05 12:39:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_pool.h"
#include "device.h"
#include "descriptor_set.h"

#include <array> // std::array
#include <list> // std::list
#include <stdexcept> // std::runtime_error

namespace scop::gfx {

using DescriptorSetPtr = DescriptorPool::DescriptorSetPtr;

VkDescriptorPool	DescriptorPool::_pool = VK_NULL_HANDLE;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Descriptor set layout for uniform buffer and combined image sampler.
*/
void	DescriptorPool::init(
	scop::core::Device& device,
	const std::vector<DescriptorSetPtr>& sets
) {
	_createDescriptorPool(device, sets);
	_allocateSets(device, sets);
}

void	DescriptorPool::destroy(scop::core::Device& device) {
	vkDestroyDescriptorPool(device.getLogicalDevice(), _pool, nullptr);
}

/* ========================================================================== */

VkDescriptorPool		DescriptorPool::getPool() noexcept {
	return _pool;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Handler for descriptor sets allocation.
*/
void	DescriptorPool::_createDescriptorPool(
	scop::core::Device& device,
	const std::vector<DescriptorSetPtr>& sets
) {
	std::array<VkDescriptorPoolSize, 3>	pool_sizes{};

	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	// pool_sizes[3].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	// pool_sizes[4].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

	for (const auto& set: sets) {
		const auto& sizes = set->getPoolSizes();
		pool_sizes[0].descriptorCount += sizes.uniform_buffer;
		pool_sizes[1].descriptorCount += sizes.combined_image_sampler;
		pool_sizes[2].descriptorCount += sizes.storage_buffer;
		// pool_sizes[3].descriptorCount += sizes.storage_image;
		// pool_sizes[4].descriptorCount += sizes.sampled_image;
	}

	VkDescriptorPoolCreateInfo	pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
	pool_info.pPoolSizes = pool_sizes.data();
	pool_info.maxSets = static_cast<uint32_t>(sets.size());

	if (vkCreateDescriptorPool(device.getLogicalDevice(), &pool_info, nullptr, &_pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool");
	}
}

/**
 * @brief Allocate every descriptor sets.
*/
void	DescriptorPool::_allocateSets(
	scop::core::Device& device,
	const std::vector<DescriptorSetPtr>& set_ptrs
) {
	std::vector<VkDescriptorSet>		descriptor_sets(set_ptrs.size());
	std::vector<VkDescriptorSetLayout>	layouts;
	layouts.reserve(set_ptrs.size());

	for (const auto& set: set_ptrs) {
		layouts.emplace_back(set->getLayout());
	}

	VkDescriptorSetAllocateInfo	alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = _pool;
	alloc_info.descriptorSetCount = static_cast<uint32_t>(descriptor_sets.size());
	alloc_info.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(device.getLogicalDevice(), &alloc_info, descriptor_sets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets");
	}

	for (std::size_t i = 0; i < set_ptrs.size(); ++i) {
		set_ptrs[i]->setDescriptors(descriptor_sets[i]);
	}
}

} // namespace scop::gfx