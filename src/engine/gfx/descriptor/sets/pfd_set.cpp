/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pfd_set.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:12:13 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 03:42:31 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pfd_set.h"
#include "device.h"
#include "game_state.h"
#include "texture.h"
#include "texture_table.h"

#include <stdexcept>

#include "debug.h"

namespace vox::gfx {

void PFDSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    BufferMetadata bufferData{};
    bufferData.m_format = sizeof(PFDUbo);
    bufferData.m_size = 1;
    bufferData.m_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferData.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    m_mvpDataBuffer.init(device, std::move(bufferData));
    m_mvpDataBuffer.map(device);

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS_FS, (u32)BindingIndex::GameData),
#if ENABLE_SHADOW_MAPPING
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS_FS, (u32)BindingIndex::ProjectorViewProj),
        _createLayoutBinding(DescriptorTypeIndex::CombinedImageSampler, ShaderVisibility::FS, (u32)BindingIndex::Shadowmap),
#endif
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = BINDING_COUNT;
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    LDEBUG("PFD descriptor set layout created");
}

void PFDSet::destroy(const Device& device) {
    m_mvpDataBuffer.unmap(device);
    m_mvpDataBuffer.destroy(device);
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("PFD descriptor set destroyed");
}

/* ========================================================================== */

void PFDSet::fill(const Device& device) {
    VkDescriptorBufferInfo gameDataInfo{};
    gameDataInfo.buffer = m_mvpDataBuffer.getBuffer();
    gameDataInfo.offset = (u32)offsetof(PFDUbo, m_gameData);
    gameDataInfo.range = sizeof(PFDUbo::m_gameData);

#if ENABLE_SHADOW_MAPPING
    VkDescriptorBufferInfo projectorInfo{};
    projectorInfo.buffer = m_mvpDataBuffer.getBuffer();
    projectorInfo.offset = (u32)offsetof(PFDUbo, m_projectorViewProj);
    projectorInfo.range = sizeof(PFDUbo::m_projectorViewProj);

    VkDescriptorImageInfo shadowmapInfo{};
    shadowmapInfo.imageLayout = TextureTable::getTexture(TextureIndex::ShadowMap)->getImageBuffer().getMetaData().m_layoutData.m_layout; // By the end of the render pass
    shadowmapInfo.imageView = TextureTable::getTexture(TextureIndex::ShadowMap)->getImageBuffer().getView();
    shadowmapInfo.sampler = TextureTable::getSampler(device, Sampler::Filter::Linear, Sampler::Border::Color, Sampler::BorderColor::WhiteFloat).getSampler();
#endif

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, gameDataInfo, (u32)BindingIndex::GameData),
#if ENABLE_SHADOW_MAPPING
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, projectorInfo, (u32)BindingIndex::ProjectorViewProj),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, shadowmapInfo, (u32)BindingIndex::Shadowmap),
#endif
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("PFD descriptor set filled");
}

void PFDSet::update(const game::GameState& state) {
    constexpr math::Vect3 SUN_COLOR = {1.0f, 1.0f, 0.33f};
    constexpr math::Vect3 MOON_COLOR = {0.5f, 0.5f, 0.5f};

    m_data.m_gameData.sunPos = state.getSunPos().xy;
    m_data.m_gameData.skyHue = math::lerp(SUN_COLOR, MOON_COLOR, std::max(0.0f, state.getSunPos().y)).toRGBA();
    m_data.m_gameData.debugIndex = state.getController().showDebug();

#if ENABLE_SHADOW_MAPPING
    constexpr float	TERRAIN_SIZE = CHUNK_SIZE * RENDER_DISTANCE;
    constexpr float	TERRAIN_HALF = TERRAIN_SIZE * 0.5f;
    constexpr float	LIGHT_DISTANCE = TERRAIN_HALF;

    const math::Mat4 projectorView = math::lookAt(
        state.getSunPos() * LIGHT_DISTANCE + state.getWorld().getOrigin(),
        state.getWorld().getOrigin(),
        WORLD_Y);
    static const math::Mat4 projectorProj = math::orthographic(
        -TERRAIN_HALF, TERRAIN_HALF,
        -TERRAIN_HALF, TERRAIN_HALF,
        0.0f, TERRAIN_SIZE);

    m_data.m_projectorViewProj = projectorProj * projectorView;
#endif

    m_mvpDataBuffer.copyFrom(&m_data);
}

} // namespace vox::gfx