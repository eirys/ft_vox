/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_set.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:12:13 by etran             #+#    #+#             */
/*   Updated: 2024/03/12 11:35:46 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mvp_set.h"
#include "device.h"
#include "game_state.h"

#include <stdexcept>

#include "debug.h"

namespace vox::gfx {

void MVPSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    _createBuffers(device, cmdBuffer);

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS, (u32)BindingIndex::TextureIndex),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::TextureSampler)
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = BINDING_COUNT;
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    LDEBUG("MVP descriptor set layout created");
}

void MVPSet::fill(const Device& device, const GameState& state) {
    (void)state;

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = m_buffer.getBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(ubo::MvpUbo);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = m_texture.getImageBuffer().getMetaData().m_layoutData.m_layout;
    imageInfo.imageView = m_texture.getImageBuffer().getView();
    imageInfo.sampler = m_texture.getSampler();

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &bufferInfo, (u32)BindingIndex::TextureIndex),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &imageInfo, (u32)BindingIndex::TextureSampler)
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("MVP descriptor set filled");
}

void MVPSet::destroy(const Device& device) {
    m_buffer.unmap(device);
    m_buffer.destroy(device);
    m_texture.destroy(device);
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("MVP descriptor set destroyed");
}

void MVPSet::update(const GameState& state) {
    m_data.pack(state.color, state.index);
    m_buffer.copyFrom(&m_data);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void MVPSet::_createBuffers(const Device& device, const ICommandBuffer* cmdBuffer) {
    // Buffers
    BufferMetadata bufferData{};
    bufferData.m_size = sizeof(ubo::MvpUbo);
    bufferData.m_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferData.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    m_buffer.init(device, std::move(bufferData));
    m_buffer.map(device);
    m_buffer.copyFrom(&m_data);

    // Samplers
    m_texture.init(device, cmdBuffer);
}

} // namespace vox::gfx