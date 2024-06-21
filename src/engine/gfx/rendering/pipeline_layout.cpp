/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_layout.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 20:40:37 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:34:37 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_layout.h"
#include "device.h"
#include "idescriptor_set.h"
#include "push_constant.h"
#include "debug.h"

#include <vector>
#include <stdexcept>

namespace vox::gfx {

void PipelineLayout::init(
    const Device& device,
    const std::vector<const IDescriptorSet*>& sets,
    const PushConstant* pushConstant
) {
    std::vector<VkDescriptorSetLayout> setLayouts;
    setLayouts.reserve(sets.size());
    for (const auto& set: sets) setLayouts.emplace_back(set->getLayout());

    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = setLayouts.size();
    layoutInfo.pSetLayouts = setLayouts.data();

    if (pushConstant != nullptr) {
        const auto& ranges = pushConstant->getRanges();
        layoutInfo.pushConstantRangeCount = ranges.size();
        layoutInfo.pPushConstantRanges = ranges.data();
    }

    if (vkCreatePipelineLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create pipeline layout.");

    m_descriptorSets.reserve(sets.size());
    for (const auto& set: sets) m_descriptorSets.emplace_back(set->getSet());

    LDEBUG("Pipeline layout created.");
}

void PipelineLayout::destroy(const Device& device) {
    vkDestroyPipelineLayout(device.getDevice(), m_layout, nullptr);
    LDEBUG("Pipeline layout destroyed.");
}

VkPipelineLayout PipelineLayout::getLayout() const noexcept {
    return m_layout;
}

const std::vector<VkDescriptorSet>& PipelineLayout::getSets() const noexcept {
    return m_descriptorSets;
}

} // namespace vox::gfx