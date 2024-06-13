/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_layout.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 20:40:37 by etran             #+#    #+#             */
/*   Updated: 2024/06/13 15:01:21 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_layout.h"
#include "device.h"
#include "icommand_buffer.h"

#include <stdexcept>

namespace vox::gfx {

void PipelineLayout::init(
    const Device& device,
    const std::vector<VkDescriptorSetLayout>& setLayouts,
    PushConstant* pushConstant
) {
    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = setLayouts.size();
    layoutInfo.pSetLayouts = setLayouts.data();

    if (pushConstant != nullptr) {
        m_pushConstant = pushConstant;

        const auto& ranges = pushConstant->getRanges();
        layoutInfo.pushConstantRangeCount = ranges.size();
        layoutInfo.pPushConstantRanges = ranges.data();
    }

    if (vkCreatePipelineLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create pipeline layout.");
}

void PipelineLayout::destroy(const Device& device) {
    if (m_pushConstant != nullptr)
        delete m_pushConstant;
    vkDestroyPipelineLayout(device.getDevice(), m_layout, nullptr);
}

/* ========================================================================== */

void PipelineLayout::updatePushConstant(const game::GameState& gameState) const {
    if (m_pushConstant == nullptr)
        return;
    m_pushConstant->update(gameState);
}

void PipelineLayout::bindPushConstantRange(const ICommandBuffer* commandBuffer) const {
    for (u32 i = 0; i < m_pushConstant->getObjectCount(); ++i) {
        const VkPushConstantRange range = m_pushConstant->getRange(i);
        const void* data = m_pushConstant->getObject(i);
        vkCmdPushConstants(
            commandBuffer->getBuffer(),
            m_layout,
            range.stageFlags, range.offset, range.size,
            data);
    }
}

VkPipelineLayout PipelineLayout::getLayout() const noexcept {
    return m_layout;
}

} // namespace vox::gfx