/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_set.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 17:03:33 by etran             #+#    #+#             */
/*   Updated: 2024/08/26 12:32:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world_set.h"
#include "device.h"
#include "texture.h"
#include "texture_table.h"
#include "sampler.h"
#include "icommand_buffer.h"
#include "game_state.h"
#include "world.h"
#include "controller.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void WorldSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    m_ubo.data[(u32)Ubo::RenderAreaSide] = game::GameState::getWorld().getSettings().rendering.getRenderAreaSide();
    m_ubo.data[(u32)Ubo::FogDistance] = ui::Controller::getFogDistance();

    BufferMetadata bufferData{};
    bufferData.m_format = sizeof(Ubo);
    bufferData.m_size = 1;
    bufferData.m_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferData.m_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    m_renderDataBuffer.init(device, std::move(bufferData));

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::Textures),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::VS, (u32)BindingIndex::Noise),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::VS, (u32)BindingIndex::Chunks),
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS_FS, (u32)BindingIndex::RenderData),
#if ENABLE_CUBEMAP
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::Cubemap),
#endif
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
    m_renderDataBuffer.destroy(device);
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("World descriptor set destroyed");
}

/* ========================================================================== */

void WorldSet::fill(const Device& device) {
    Sampler::Metadata gameTexSamplerInfo;
    gameTexSamplerInfo.m_filter = Sampler::Filter::Nearest;
    gameTexSamplerInfo.m_mipMode = Sampler::MipMode::Linear;
    gameTexSamplerInfo.m_border = Sampler::Border::Color;
    gameTexSamplerInfo.m_borderColor = Sampler::BorderColor::WhiteFloat;
    gameTexSamplerInfo.m_maxLod = (float)TextureTable::getTexture(TextureIndex::GameTexture)->getImageBuffer().getMetaData().m_mipCount;

    VkDescriptorImageInfo gameSamplerInfo{};
    gameSamplerInfo.imageLayout = TextureTable::getTexture(TextureIndex::GameTexture)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    gameSamplerInfo.imageView = TextureTable::getTexture(TextureIndex::GameTexture)->getImageBuffer().getView();
    gameSamplerInfo.sampler = TextureTable::getSampler(device, gameTexSamplerInfo).getSampler();

    VkDescriptorImageInfo noiseSamplerInfo{};
    noiseSamplerInfo.imageLayout = TextureTable::getTexture(TextureIndex::PerlinNoise)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    noiseSamplerInfo.imageView = TextureTable::getTexture(TextureIndex::PerlinNoise)->getImageBuffer().getView();
    noiseSamplerInfo.sampler = TextureTable::getSampler(device, Sampler::Filter::Linear, Sampler::Border::Color, Sampler::BorderColor::BlackInt).getSampler();

    VkDescriptorImageInfo chunkTexInfo{};
    chunkTexInfo.imageLayout = TextureTable::getTexture(TextureIndex::ChunkData)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    chunkTexInfo.imageView = TextureTable::getTexture(TextureIndex::ChunkData)->getImageBuffer().getView();
    chunkTexInfo.sampler = TextureTable::getSampler(device, Sampler::Filter::Nearest, Sampler::Border::Color, Sampler::BorderColor::BlackInt).getSampler();

    VkDescriptorBufferInfo renderDataInfo{};
    renderDataInfo.buffer = m_renderDataBuffer.getBuffer();
    renderDataInfo.offset = 0;
    renderDataInfo.range = sizeof(Ubo);

#if ENABLE_CUBEMAP
    Sampler::Metadata samplerProperties;
    samplerProperties.m_filter = Sampler::Filter::Linear;
    samplerProperties.m_mipMode = Sampler::MipMode::Linear;
    samplerProperties.m_border = Sampler::Border::Edge;
    samplerProperties.m_borderColor = Sampler::BorderColor::WhiteFloat;
    samplerProperties.m_enableAnisotropy = true;
    samplerProperties.m_maxLod = (float)TextureTable::getTexture(TextureIndex::SkyCubemap)->getImageBuffer().getMetaData().m_mipCount;

    VkDescriptorImageInfo skyboxInfo{};
    skyboxInfo.imageLayout = TextureTable::getTexture(TextureIndex::SkyCubemap)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    skyboxInfo.imageView = TextureTable::getTexture(TextureIndex::SkyCubemap)->getImageBuffer().getView();
    skyboxInfo.sampler = TextureTable::getSampler(device, samplerProperties).getSampler();
#endif

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, gameSamplerInfo, (u32)BindingIndex::Textures),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, noiseSamplerInfo, (u32)BindingIndex::Noise),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, chunkTexInfo, (u32)BindingIndex::Chunks),
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, renderDataInfo, (u32)BindingIndex::RenderData),
#if ENABLE_CUBEMAP
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, skyboxInfo, (u32)BindingIndex::Cubemap),
#endif
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("World descriptor set filled");
}

void WorldSet::update(const Device& device, const ICommandBuffer* cmdBuffer) {
    //TODO update before transfper
    Buffer stagingBuffer = m_renderDataBuffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(&m_ubo);
    stagingBuffer.unmap(device);

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    m_renderDataBuffer.copyBuffer(cmdBuffer, stagingBuffer);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);
    stagingBuffer.destroy(device);
}

} // namespace vox::gfx