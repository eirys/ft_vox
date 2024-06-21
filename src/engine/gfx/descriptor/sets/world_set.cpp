/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_set.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 17:03:33 by etran             #+#    #+#             */
/*   Updated: 2024/06/20 16:27:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world_set.h"
#include "device.h"
#include "texture.h"
#include "texture_table.h"
#include "sampler.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void WorldSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::Textures),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::VS, (u32)BindingIndex::Noise),
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
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("World descriptor set destroyed");
}

/* ========================================================================== */

void WorldSet::fill(const Device& device) {
    Sampler::Metadata gameTexSampler;
    gameTexSampler.m_filter = Sampler::Filter::Nearest;
    gameTexSampler.m_mipMode = Sampler::MipMode::Linear;
    gameTexSampler.m_border = Sampler::Border::Edge;
    gameTexSampler.m_borderColor = Sampler::BorderColor::WhiteFloat;
    gameTexSampler.m_maxLod = (float)TextureTable::getTexture(TextureIndex::GameTexture)->getImageBuffer().getMetaData().m_mipCount;

    VkDescriptorImageInfo gameSamplerInfo{};
    gameSamplerInfo.imageLayout = TextureTable::getTexture(TextureIndex::GameTexture)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    gameSamplerInfo.imageView = TextureTable::getTexture(TextureIndex::GameTexture)->getImageBuffer().getView();
    gameSamplerInfo.sampler = TextureTable::getSampler(device, gameTexSampler).getSampler();

    VkDescriptorImageInfo noiseSamplerInfo{};
    noiseSamplerInfo.imageLayout = TextureTable::getTexture(TextureIndex::PerlinNoise)->getImageBuffer().getMetaData().m_layoutData.m_layout;
    noiseSamplerInfo.imageView = TextureTable::getTexture(TextureIndex::PerlinNoise)->getImageBuffer().getView();
    noiseSamplerInfo.sampler = TextureTable::getSampler(device, Sampler::Filter::Linear, Sampler::Border::Color, Sampler::BorderColor::BlackInt).getSampler();

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
#if ENABLE_CUBEMAP
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, skyboxInfo, (u32)BindingIndex::Cubemap),
#endif
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("World descriptor set filled");
}

} // namespace vox::gfx