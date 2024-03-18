/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_set.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 17:03:33 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 15:55:05 by etran            ###   ########.fr       */
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
    m_chunkDataSampler.init(device, cmdBuffer);
    m_gameTextureSampler.init(device, cmdBuffer);
    m_gameTextureSampler.fill(device, cmdBuffer);

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::VS, (u32)BindingIndex::BlockPos),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::Textures),
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
    m_chunkDataSampler.destroy(device);
    m_gameTextureSampler.destroy(device);
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("World descriptor set destroyed");
}

/* ========================================================================== */

void WorldSet::fill(const Device& device) {
    VkDescriptorImageInfo chunkSamplerInfo{};
    chunkSamplerInfo.imageLayout = m_chunkDataSampler.getImageBuffer().getMetaData().m_layoutData.m_layout;
    chunkSamplerInfo.imageView = m_chunkDataSampler.getImageBuffer().getView();
    chunkSamplerInfo.sampler = m_chunkDataSampler.getSampler();

    VkDescriptorImageInfo gameSamplerInfo{};
    gameSamplerInfo.imageLayout = m_gameTextureSampler.getImageBuffer().getMetaData().m_layoutData.m_layout;
    gameSamplerInfo.imageView = m_gameTextureSampler.getImageBuffer().getView();
    gameSamplerInfo.sampler = m_gameTextureSampler.getSampler();

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &chunkSamplerInfo, (u32)BindingIndex::BlockPos),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &gameSamplerInfo, (u32)BindingIndex::Textures),
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("WorldSet descriptor set filled");
}

void WorldSet::update(const Device& device, const game::GameState& state, const ICommandBuffer* cmdBuffer) {
    m_chunkDataSampler.fill(device, cmdBuffer, &state.getWorld().getChunks());
}

} // namespace vox::gfx