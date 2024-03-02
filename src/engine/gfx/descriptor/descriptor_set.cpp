/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:54:32 by etran             #+#    #+#             */
/*   Updated: 2024/02/29 22:39:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_set.h"
#include "device.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

DescriptorSet::DescriptorSet(const DescriptorSetIndex index): m_index(index) {}

/* ========================================================================== */

void DescriptorSet::destroy(const Device& device) {
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);
}

void DescriptorSet::setDescriptorSet(const VkDescriptorSet set) noexcept {
    m_set = set;
}

/* ========================================================================== */

VkDescriptorSetLayout DescriptorSet::getLayout() const noexcept {
    return m_layout;
}

VkDescriptorSet DescriptorSet::getSet() const noexcept {
    return m_set;
}

const DescriptorSet::PoolSizes& DescriptorSet::getSizes() const noexcept {
    return m_poolSizes;
}

DescriptorSetIndex DescriptorSet::getSetIndex() const noexcept {
    return m_index;
}

/* ========================================================================== */
/*                                  PROTECTED                                 */
/* ========================================================================== */

VkDescriptorSetLayoutBinding DescriptorSet::_createLayoutBinding(
    const DescriptorTypeIndex typeIndex,
    const ShaderVisibility shaderStage,
    const u32 bindingIndex,
    const u32 count
) {
    VkDescriptorSetLayoutBinding layoutBinding = {};
    layoutBinding.binding = bindingIndex;
    layoutBinding.stageFlags = (VkShaderStageFlagBits)shaderStage;
    layoutBinding.descriptorCount = count;
    layoutBinding.descriptorType = DESCRIPTOR_TYPES[(u32)typeIndex];

    ++m_poolSizes[(u32)typeIndex].descriptorCount;

    return layoutBinding;
}

VkWriteDescriptorSet DescriptorSet::_createWriteDescriptorSet(
    const DescriptorTypeIndex typeIndex,
    const void* descriptorInfo,
    const u32 bindingIndex,
    const u32 count
) const {
    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = m_set;
    writeDescriptorSet.dstBinding = bindingIndex;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = count;
    writeDescriptorSet.descriptorType = DESCRIPTOR_TYPES[(u32)typeIndex];

    switch (typeIndex) {
        case DescriptorTypeIndex::CombinedImageSampler:
        case DescriptorTypeIndex::SampledImage:
        case DescriptorTypeIndex::StorageImage:
            writeDescriptorSet.pImageInfo = (VkDescriptorImageInfo*)descriptorInfo;
            break;

        case DescriptorTypeIndex::UniformBuffer:
        case DescriptorTypeIndex::StorageBuffer:
            writeDescriptorSet.pBufferInfo = (VkDescriptorBufferInfo*)descriptorInfo;
            break;
    }

    return writeDescriptorSet;
}

} // namespace vox::gfx