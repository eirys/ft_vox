/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gbuffer_set.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 23:57:17 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 01:34:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gbuffer_set.h"
#include "texture.h"
#include "device.h"
#include "texture_table.h"
#include "debug.h"

namespace vox::gfx {

void GBufferSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::PositionTexture),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::NormalTexture),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::AlbedoTexture),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::NormalViewTexture),
#if ENABLE_SSAO
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::SsaoTexture),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::SsaoBlur),
#endif
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = BINDING_COUNT;
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
        throw std::runtime_error("failed to create gbuffer descriptor set layout");

    LDEBUG("GBuffer descriptor set layout created");
}

void GBufferSet::destroy(const Device& device) {
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("GBuffer descriptor set destroyed");
}

void GBufferSet::fill(const Device& device) {
    const VkSampler sampler = TextureTable::getSampler(device, Sampler::Filter::Nearest, Sampler::Border::Edge, Sampler::BorderColor::WhiteFloat).getSampler();

    VkDescriptorImageInfo positionTextureInfo{};
    positionTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::GBufferPosition)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    positionTextureInfo.imageView = TextureTable::getTexture(TextureIndex::GBufferPosition)->getImageBuffer().getView();
    positionTextureInfo.sampler = sampler;

    VkDescriptorImageInfo normalTextureInfo{};
    normalTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::GBufferNormal)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    normalTextureInfo.imageView = TextureTable::getTexture(TextureIndex::GBufferNormal)->getImageBuffer().getView();
    normalTextureInfo.sampler = sampler;

    VkDescriptorImageInfo albedoTextureInfo{};
    albedoTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::GBufferAlbedo)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    albedoTextureInfo.imageView = TextureTable::getTexture(TextureIndex::GBufferAlbedo)->getImageBuffer().getView();
    albedoTextureInfo.sampler = sampler;

    VkDescriptorImageInfo normalViewTextureInfo{};
    normalViewTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::GBufferNormalView)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    normalViewTextureInfo.imageView = TextureTable::getTexture(TextureIndex::GBufferNormalView)->getImageBuffer().getView();
    normalViewTextureInfo.sampler = sampler;

#if ENABLE_SSAO
    VkDescriptorImageInfo ssaoTextureInfo{};
    ssaoTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::GBufferSSAO)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    ssaoTextureInfo.imageView = TextureTable::getTexture(TextureIndex::GBufferSSAO)->getImageBuffer().getView();
    ssaoTextureInfo.sampler = sampler;

    VkDescriptorImageInfo ssaoBlurTextureInfo{};
    ssaoBlurTextureInfo.imageLayout = TextureTable::getTexture(TextureIndex::GBufferSSAOBlur)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    ssaoBlurTextureInfo.imageView = TextureTable::getTexture(TextureIndex::GBufferSSAOBlur)->getImageBuffer().getView();
    ssaoBlurTextureInfo.sampler = sampler;
#endif

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, positionTextureInfo, (u32)BindingIndex::PositionTexture),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, normalTextureInfo, (u32)BindingIndex::NormalTexture),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, albedoTextureInfo, (u32)BindingIndex::AlbedoTexture),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, normalViewTextureInfo, (u32)BindingIndex::NormalViewTexture),
#if ENABLE_SSAO
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, ssaoTextureInfo, (u32)BindingIndex::SsaoTexture),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, ssaoBlurTextureInfo, (u32)BindingIndex::SsaoBlur),
#endif
    };

    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("GBuffer descriptor set filled");
}

} // namespace vox::gfx