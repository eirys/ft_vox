/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:00:20 by etran             #+#    #+#             */
/*   Updated: 2024/03/01 00:03:24 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_pool.h"
#include "device.h"
#include "descriptor_table.h"
#include "idescriptor_set.h"

#include <stdexcept>
#include <array>
#include <vector>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void DescriptorPool::init(const Device& device, DescriptorTable& table) {
    _createPool(device, table);
    _allocatePool(device, table);
}

void DescriptorPool::destroy(const Device& device) {
    vkDestroyDescriptorPool(device.getDevice(), m_pool, nullptr);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void DescriptorPool::_createPool(const Device& device, const DescriptorTable& table) {
    IDescriptorSet::PoolSizes possiblePoolSizes{};

    for (u32 typeIndex = (u32)DescriptorTypeIndex::First; typeIndex <= (u32)DescriptorTypeIndex::Last; ++typeIndex)
        possiblePoolSizes[typeIndex].type = DESCRIPTOR_TYPES[typeIndex];

    // possiblePoolSizes[(u32)DescriptorTypeIndex::UniformBuffer].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    // possiblePoolSizes[(u32)DescriptorTypeIndex::CombinedImageSampler].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // possiblePoolSizes[(u32)DescriptorTypeIndex::StorageBuffer].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    // possiblePoolSizes[(u32)DescriptorTypeIndex::StorageImage].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    // possiblePoolSizes[(u32)DescriptorTypeIndex::SampledImage].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

    for (u32 setIndex = 0; setIndex < DESCRIPTOR_TABLE_SIZE; ++setIndex) {
        const IDescriptorSet::PoolSizes& setSize = table[(DescriptorSetIndex)setIndex]->getSizes();

        for (u32 typeIndex = (u32)DescriptorTypeIndex::First; typeIndex <= (u32)DescriptorTypeIndex::Last; ++typeIndex)
            possiblePoolSizes[typeIndex].descriptorCount += setSize[typeIndex].descriptorCount;

        // possiblePoolSizes[0].descriptorCount += setSize.uniformBuffer;
        // possiblePoolSizes[1].descriptorCount += setSize.combinedImageSampler;
        // possiblePoolSizes[2].descriptorCount += setSize.storageBuffer;
        // possiblePoolSizes[3].descriptorCount += setSize.storageImage;
        // possiblePoolSizes[4].descriptorCount += setSize.sampledImage;
    }

    std::vector<VkDescriptorPoolSize> poolSizes;
    for (const auto& poolSize: possiblePoolSizes) {
        if (poolSize.descriptorCount != 0) poolSizes.emplace_back(poolSize);
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = (u32)poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = DESCRIPTOR_TABLE_SIZE;

    if (vkCreateDescriptorPool(device.getDevice(), &poolInfo, nullptr, &m_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool");
    }
}

void DescriptorPool::_allocatePool(const Device& device, DescriptorTable& table) {
    std::vector<VkDescriptorSet>        descriptorSets(DESCRIPTOR_TABLE_SIZE);
    std::vector<VkDescriptorSetLayout>  layouts;
    layouts.reserve(DESCRIPTOR_TABLE_SIZE);

    for (u32 i = 0; i < DESCRIPTOR_TABLE_SIZE; ++i)
        layouts.emplace_back(table[(DescriptorSetIndex)i]->getLayout());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_pool;
    allocInfo.descriptorSetCount = DESCRIPTOR_TABLE_SIZE;
    allocInfo.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(device.getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets");
    }

    for (u32 i = 0; i < DESCRIPTOR_TABLE_SIZE; ++i)
        table[(DescriptorSetIndex)i]->setDescriptorSet(descriptorSets[i]);
}

} // namespace vox::gfx