/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssao_sets.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:55:01 by etran             #+#    #+#             */
/*   Updated: 2024/06/25 14:49:59 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ssao_sets.h"
#include "device.h"
#include "texture.h"
#include "icommand_buffer.h"
#include "texture_table.h"
#include "maths.h"
#include "debug.h"

#include <random>
#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                    SSAO                                    */
/* ========================================================================== */

void SSAOSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    m_samplesBuffer = _createSamplesBuffer(device, cmdBuffer);

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::FS, (u32)BindingIndex::Samples),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::NoiseTexture),
#if ENABLE_SSAO
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::PositionViewTexture),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::NormalViewTexture),
#endif
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = BINDING_COUNT;
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
        throw std::runtime_error("failed to create gbuffer descriptor set layout");

    LDEBUG("SSAO descriptor set layout created");
}

void SSAOSet::destroy(const Device& device) {
    m_samplesBuffer.destroy(device);

    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("SSAO descriptor set destroyed");
}

void SSAOSet::fill(const Device& device) {
    VkDescriptorBufferInfo samplesInfo{};
    samplesInfo.buffer = m_samplesBuffer.getBuffer();
    samplesInfo.offset = 0;
    samplesInfo.range = m_samplesBuffer.getMetadata().m_size * m_samplesBuffer.getMetadata().m_format;

    const VkSampler sampler = TextureTable::getSampler(device, Sampler::Filter::Nearest, Sampler::Border::Edge, Sampler::BorderColor::WhiteFloat).getSampler();

    VkDescriptorImageInfo noiseTextureInfo{};
    noiseTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::PerlinNoise)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    noiseTextureInfo.imageView = TextureTable::getTexture(TextureIndex::PerlinNoise)->getImageBuffer().getView();
    noiseTextureInfo.sampler = sampler;

#if ENABLE_SSAO
    VkDescriptorImageInfo posViewTextureInfo{};
    posViewTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::GBufferPositionView)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    posViewTextureInfo.imageView = TextureTable::getTexture(TextureIndex::GBufferPositionView)->getImageBuffer().getView();
    posViewTextureInfo.sampler = sampler;

    VkDescriptorImageInfo normalViewTextureInfo{};
    normalViewTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::GBufferNormalView)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    normalViewTextureInfo.imageView = TextureTable::getTexture(TextureIndex::GBufferNormalView)->getImageBuffer().getView();
    normalViewTextureInfo.sampler = sampler;
#endif

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, samplesInfo, (u32)BindingIndex::Samples),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, noiseTextureInfo, (u32)BindingIndex::NoiseTexture),
#if ENABLE_SSAO
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, posViewTextureInfo, (u32)BindingIndex::PositionViewTexture),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, normalViewTextureInfo, (u32)BindingIndex::NormalViewTexture),
#endif
    };

    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("SSAO descriptor set filled");
}

Buffer SSAOSet::_createSamplesBuffer(const Device& device, const ICommandBuffer* transferBuffer) {
    Buffer buffer{};

    std::uniform_real_distribution<float>   randomDistance(0.0f, 1.0f);
    std::default_random_engine              gen;

    std::vector<math::Vect3> ssaoKernel;
    ssaoKernel.reserve(SSAO_KERNEL_SIZE);
    for (u32 i = 0; i < SSAO_KERNEL_SIZE; ++i) {
        math::Vect3 sample(
            randomDistance(gen) * 2.0f - 1.0f,
            randomDistance(gen) * 2.0f - 1.0f,
            randomDistance(gen));
        sample = math::normalize(sample);
        sample *= randomDistance(gen);

        f32 scale = f32(i) / f32(SSAO_KERNEL_SIZE);
        scale = math::lerp(0.1f, 1.0f, scale * scale);

        ssaoKernel.emplace_back(sample * scale);
    }

    BufferMetadata samplesBufferData{};
    samplesBufferData.m_format = sizeof(math::Vect3);
    samplesBufferData.m_size = ssaoKernel.size();
    samplesBufferData.m_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    samplesBufferData.m_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    buffer.init(device, std::move(samplesBufferData));

    Buffer  stagingBuffer = buffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(ssaoKernel.data());
    stagingBuffer.unmap(device);

    transferBuffer->reset();
    transferBuffer->startRecording();
    buffer.copyBuffer(transferBuffer, stagingBuffer);
    transferBuffer->stopRecording();
    transferBuffer->awaitEndOfRecording(device);

    stagingBuffer.destroy(device);

    return buffer;
}

/* ========================================================================== */
/*                                  SSAO BLUR                                 */
/* ========================================================================== */

void SSAOBlurSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
#if ENABLE_SSAO
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::Texture),
#endif
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = BINDING_COUNT;
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
        throw std::runtime_error("failed to create gbuffer descriptor set layout");

    LDEBUG("SSAO blur descriptor set layout created");
}

void SSAOBlurSet::destroy(const Device& device) {
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("SSAO blur descriptor set destroyed");
}

void SSAOBlurSet::fill(const Device& device) {
#if ENABLE_SSAO
    const VkSampler sampler = TextureTable::getSampler(device, Sampler::Filter::Nearest, Sampler::Border::Edge, Sampler::BorderColor::WhiteFloat).getSampler();

    VkDescriptorImageInfo ssaoTextureInfo{};
    ssaoTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::GBufferSSAO)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    ssaoTextureInfo.imageView = TextureTable::getTexture(TextureIndex::GBufferSSAO)->getImageBuffer().getView();
    ssaoTextureInfo.sampler = sampler;
#endif

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
#if ENABLE_SSAO
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, ssaoTextureInfo, (u32)BindingIndex::Texture),
#endif
    };

    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("SSAO blur descriptor set filled");
}

} // namespace vox::gfx