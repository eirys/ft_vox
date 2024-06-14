/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gbuffer_set.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 23:57:17 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 17:19:55 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gbuffer_set.h"
#include "gbuffer_textures.h"
#include "device.h"
#include "debug.h"

namespace vox::gfx {

void GBufferSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    m_textures[(u32)Texture::PositionTexture] = new PositionTexture();
    m_textures[(u32)Texture::NormalTexture] = new NormalTexture();
    m_textures[(u32)Texture::AlbedoTexture] = new AlbedoTexture();

    for (u32 i = 0; i < TEXTURE_COUNT; ++i) m_textures[i]->init(device);

    VkSampler sampler = m_textures[(u32)Texture::PositionTexture]->getSampler();
    m_textures[(u32)Texture::NormalTexture]->fill(device, cmdBuffer, &sampler);
    m_textures[(u32)Texture::AlbedoTexture]->fill(device, cmdBuffer, &sampler);

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::PositionTexture),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::NormalTexture),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::AlbedoTexture) };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = BINDING_COUNT;
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
        throw std::runtime_error("failed to create gbuffer descriptor set layout");

    LDEBUG("GBuffer descriptor set layout created");
}

void GBufferSet::destroy(const Device& device) {
    for (u32 i = 0; i < TEXTURE_COUNT; ++i) {
        m_textures[i]->destroy(device);
        delete m_textures[i];
    }

    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("GBuffer descriptor set destroyed");
}

void GBufferSet::fill(const Device& device) {
    VkDescriptorImageInfo positionTextureInfo{};
    positionTextureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    positionTextureInfo.imageView = m_textures[(u32)Texture::PositionTexture]->getImageBuffer().getView();
    positionTextureInfo.sampler = m_textures[(u32)Texture::PositionTexture]->getSampler();

    VkDescriptorImageInfo normalTextureInfo{};
    normalTextureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    normalTextureInfo.imageView = m_textures[(u32)Texture::NormalTexture]->getImageBuffer().getView();
    normalTextureInfo.sampler = m_textures[(u32)Texture::NormalTexture]->getSampler();

    VkDescriptorImageInfo albedoTextureInfo{};
    albedoTextureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    albedoTextureInfo.imageView = m_textures[(u32)Texture::AlbedoTexture]->getImageBuffer().getView();
    albedoTextureInfo.sampler = m_textures[(u32)Texture::AlbedoTexture]->getSampler();

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &positionTextureInfo, (u32)BindingIndex::PositionTexture),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &normalTextureInfo, (u32)BindingIndex::NormalTexture),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &albedoTextureInfo, (u32)BindingIndex::AlbedoTexture)
    };

    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("GBuffer descriptor set filled");
}

/* ========================================================================== */

const ImageBuffer& GBufferSet::getImageBuffer(const u32 index) const noexcept {
    return m_textures[index]->getImageBuffer();
}

} // namespace vox::gfx