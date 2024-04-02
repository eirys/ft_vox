/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_set.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 17:03:33 by etran             #+#    #+#             */
/*   Updated: 2024/04/01 00:35:17 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "world_set.h"
#include "device.h"
#include "game_state.h"
#include "chunk_data_sampler.h"
#include "game_texture_sampler.h"
#include "skybox_sampler.h"
#include "perlin_noise_sampler.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void WorldSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    m_textures[(u32)Texture::ChunkData] = new ChunkDataSampler();
    m_textures[(u32)Texture::GameTexture] = new GameTextureSampler();
    m_textures[(u32)Texture::PerlinNoise] = new PerlinNoiseSampler();

#if ENABLE_CUBEMAP
    m_textures[(u32)Texture::Skybox] = new SkyboxSampler();
#endif

    for (u32 i = 0; i < TEXTURE_COUNT; ++i) m_textures[i]->init(device, cmdBuffer);

    m_textures[(u32)Texture::GameTexture]->fill(device, cmdBuffer);
    m_textures[(u32)Texture::PerlinNoise]->fill(device, cmdBuffer);

#if ENABLE_CUBEMAP
    m_textures[(u32)Texture::Skybox]->fill(device, cmdBuffer);
#endif

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::VS, (u32)BindingIndex::BlockPos),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::Textures),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::VS, (u32)BindingIndex::Noise),
#if ENABLE_CUBEMAP
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::Skybox),
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
    for (u32 i = 0; i < TEXTURE_COUNT; ++i) {
        m_textures[i]->destroy(device);
        delete m_textures[i];
    }

    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("World descriptor set destroyed");
}

/* ========================================================================== */

void WorldSet::fill(const Device& device) {
    LDEBUG("Filling WorldSet descriptor set");

    VkDescriptorImageInfo chunkSamplerInfo{};
    chunkSamplerInfo.imageLayout = m_textures[(u32)Texture::ChunkData]->getImageBuffer().getMetaData().m_layoutData.m_layout;
    chunkSamplerInfo.imageView = m_textures[(u32)Texture::ChunkData]->getImageBuffer().getView();
    chunkSamplerInfo.sampler = m_textures[(u32)Texture::ChunkData]->getSampler();

    VkDescriptorImageInfo gameSamplerInfo{};
    gameSamplerInfo.imageLayout = m_textures[(u32)Texture::GameTexture]->getImageBuffer().getMetaData().m_layoutData.m_layout;
    gameSamplerInfo.imageView = m_textures[(u32)Texture::GameTexture]->getImageBuffer().getView();
    gameSamplerInfo.sampler = m_textures[(u32)Texture::GameTexture]->getSampler();

    VkDescriptorImageInfo noiseSamplerInfo{};
    noiseSamplerInfo.imageLayout = m_textures[(u32)Texture::PerlinNoise]->getImageBuffer().getMetaData().m_layoutData.m_layout;
    noiseSamplerInfo.imageView = m_textures[(u32)Texture::PerlinNoise]->getImageBuffer().getView();
    noiseSamplerInfo.sampler = m_textures[(u32)Texture::PerlinNoise]->getSampler();

#if ENABLE_CUBEMAP
    VkDescriptorImageInfo skyboxInfo{};
    skyboxInfo.imageLayout = m_textures[(u32)Texture::Skybox]->getImageBuffer().getMetaData().m_layoutData.m_layout;
    skyboxInfo.imageView = m_textures[(u32)Texture::Skybox]->getImageBuffer().getView();
    skyboxInfo.sampler = m_textures[(u32)Texture::Skybox]->getSampler();
#endif

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &chunkSamplerInfo, (u32)BindingIndex::BlockPos),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &gameSamplerInfo, (u32)BindingIndex::Textures),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &noiseSamplerInfo, (u32)BindingIndex::Noise),
#if ENABLE_CUBEMAP
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &skyboxInfo, (u32)BindingIndex::Skybox),
#endif
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("WorldSet descriptor set filled");
}

void WorldSet::update(const Device& device, const game::GameState& state, const ICommandBuffer* cmdBuffer) {
    m_textures[(u32)Texture::ChunkData]->fill(device, cmdBuffer, &state.getWorld().getChunks());
}

} // namespace vox::gfx