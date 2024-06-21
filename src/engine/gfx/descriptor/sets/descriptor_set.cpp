/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:54:32 by etran             #+#    #+#             */
/*   Updated: 2024/06/20 19:08:21 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_set.h"
#include "device.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

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

/* ========================================================================== */
/*                                  PROTECTED                                 */
/* ========================================================================== */

VkDescriptorSetLayoutBinding DescriptorSet::_createLayoutBinding(
    const DescriptorTypeIndex typeIndex,
    const ShaderVisibility shaderStage,
    const u32 bindingIndex
) {
    VkDescriptorSetLayoutBinding layoutBinding = {};
    layoutBinding.binding = bindingIndex;
    layoutBinding.stageFlags = (VkShaderStageFlagBits)shaderStage;
    layoutBinding.descriptorCount = 1;
    layoutBinding.descriptorType = DESCRIPTOR_TYPES[(u32)typeIndex];

    ++m_poolSizes[(u32)typeIndex].descriptorCount;

    return layoutBinding;
}

VkWriteDescriptorSet DescriptorSet::_createWriteDescriptorSet(
    const DescriptorTypeIndex typeIndex,
    const VkDescriptorBufferInfo& bufferInfo,
    const u32 bindingIndex
) const {
    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = m_set;
    writeDescriptorSet.dstBinding = bindingIndex;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.descriptorType = DESCRIPTOR_TYPES[(u32)typeIndex];
    writeDescriptorSet.pBufferInfo = &bufferInfo;

    return writeDescriptorSet;
}

VkWriteDescriptorSet DescriptorSet::_createWriteDescriptorSet(
    const DescriptorTypeIndex typeIndex,
    const VkDescriptorImageInfo& imageInfo,
    const u32 bindingIndex
) const {
    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.dstSet = m_set;
    writeDescriptorSet.dstBinding = bindingIndex;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.descriptorType = DESCRIPTOR_TYPES[(u32)typeIndex];
    writeDescriptorSet.pImageInfo = &imageInfo;

    return writeDescriptorSet;
}

} // namespace vox::gfx