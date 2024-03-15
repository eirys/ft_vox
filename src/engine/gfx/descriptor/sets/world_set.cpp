/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_set.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 17:03:33 by etran             #+#    #+#             */
/*   Updated: 2024/03/15 21:20:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world_set.h"
#include "device.h"
#include "game_state.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

constexpr u32 BUFFERSIZE = WORLD_SIZE * CHUNK_AREA;

void WorldSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    BufferMetadata bufferData{};
    bufferData.m_size = BUFFERSIZE;
    bufferData.m_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferData.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    m_blockPosBuffer.init(device, std::move(bufferData));
    m_blockPosBuffer.map(device);

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS, (u32)BindingIndex::BlockPos),
        // _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::Textures)
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = BINDING_COUNT;
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    LDEBUG("World descriptor set layout created");
}

void WorldSet::destroy(const Device& device) {
    m_blockPosBuffer.unmap(device);
    m_blockPosBuffer.destroy(device);
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("World descriptor set destroyed");
}

/* ========================================================================== */

void WorldSet::fill(const Device& device) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = m_blockPosBuffer.getBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = BUFFERSIZE;

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &bufferInfo, (u32)BindingIndex::BlockPos),
        // _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &imageInfo, (u32)BindingIndex::TextureSampler)
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("WorldSet descriptor set filled");
}

void WorldSet::update(const game::GameState& state) {
    static bool updated = false;

    if (updated)
        return;

    const auto& chunks = state.getWorld().getChunks();

    for (u32 i = 0; i < WORLD_SIZE; i++) {
        const auto& heights = chunks[i].getHeights();
        m_blockPosBuffer.copyFrom(heights.data(), CHUNK_AREA, i * CHUNK_AREA);
    }

    updated = true;
}

} // namespace vox::gfx