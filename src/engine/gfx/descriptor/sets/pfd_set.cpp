/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pfd_set.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:12:13 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 21:10:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pfd_set.h"
#include "device.h"
#include "game_state.h"
#include "matrix.h"
#include "maths.h"
#include "shadowmap_sampler.h"

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

#if ENABLE_SHADOW_MAPPING
    m_textures[(u32)Texture::Shadowmap] = new ShadowmapSampler();
#endif

    for (u32 i = 0; i < TEXTURE_COUNT; ++i) m_textures[i]->init(device);

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS, (u32)BindingIndex::CameraViewProj),
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS_FS, (u32)BindingIndex::GameData),
#if ENABLE_SHADOW_MAPPING
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS, (u32)BindingIndex::ProjectorViewProj),
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
    for (u32 i = 0; i < TEXTURE_COUNT; ++i) {
        m_textures[i]->destroy(device);
        delete m_textures[i];
    }

    m_mvpDataBuffer.unmap(device);
    m_mvpDataBuffer.destroy(device);
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("PFD descriptor set destroyed");
}

/* ========================================================================== */

void PFDSet::fill(const Device& device) {
    VkDescriptorBufferInfo cameraInfo{};
    cameraInfo.buffer = m_mvpDataBuffer.getBuffer();
    cameraInfo.offset = (VkDeviceSize)PFDUbo::Offset::CameraViewProj;
    cameraInfo.range = sizeof(PFDUbo::m_cameraViewProj);

    VkDescriptorBufferInfo gameDataInfo{};
    gameDataInfo.buffer = m_mvpDataBuffer.getBuffer();
    gameDataInfo.offset = (VkDeviceSize)PFDUbo::Offset::GameData;
    gameDataInfo.range = sizeof(PFDUbo::m_gameData);

#if ENABLE_SHADOW_MAPPING
    VkDescriptorBufferInfo projectorInfo{};
    projectorInfo.buffer = m_mvpDataBuffer.getBuffer();
    projectorInfo.offset = (VkDeviceSize)PFDUbo::Offset::ProjectorViewProj;
    projectorInfo.range = sizeof(PFDUbo::m_projectorViewProj);

    VkDescriptorImageInfo shadowmapInfo{};
    shadowmapInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL; // By the end of the render pass
    shadowmapInfo.imageView = m_textures[(u32)Texture::Shadowmap]->getImageBuffer().getView();
    shadowmapInfo.sampler = m_textures[(u32)Texture::Shadowmap]->getSampler();
#endif

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &cameraInfo, (u32)BindingIndex::CameraViewProj),
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &gameDataInfo, (u32)BindingIndex::GameData),
#if ENABLE_SHADOW_MAPPING
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &projectorInfo, (u32)BindingIndex::ProjectorViewProj),
        _createWriteDescriptorSet(DescriptorTypeIndex::CombinedImageSampler, &shadowmapInfo, (u32)BindingIndex::Shadowmap),
#endif
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("PFD descriptor set filled");
}

void PFDSet::update(const game::GameState& state) {
    const ui::Camera&   camera = state.getController().getCamera();
    static const f32    fovRadians = math::radians(camera.m_fov);

    m_data.m_cameraViewProj.view = math::lookAt(camera.m_position, camera.m_front, camera.m_up, camera.m_right);
    m_data.m_cameraViewProj.proj = math::perspective(
        fovRadians,
        ui::Camera::ASPECT_RATIO,
        ui::Camera::NEAR_PLANE,
        ui::Camera::FAR_PLANE);

    constexpr math::Vect3 SUN_COLOR = {1.0f, 1.0f, 0.33f};
    constexpr math::Vect3 MOON_COLOR = {0.5f, 0.5f, 0.5f};

    m_data.m_gameData.sunPos = state.getSunPos().xy;
    m_data.m_gameData.skyHue = math::lerp(SUN_COLOR, MOON_COLOR, std::max(0.0f, state.getSunPos().y)).toRGBA();

#if ENABLE_SHADOW_MAPPING
    constexpr float	TERRAIN_SIZE = CHUNK_SIZE * RENDER_DISTANCE;
    constexpr float	TERRAIN_HALF = TERRAIN_SIZE / 2.0f;
    constexpr math::Vect3 ORIGIN = WORLD_ORIGIN;

    const math::Vect3 lighPosition = state.getSunPos() * TERRAIN_HALF + ORIGIN;

    m_data.m_projectorViewProj.view = math::lookAt(lighPosition, ORIGIN, { 0.0f, 1.0f, 0.0f });
    m_data.m_projectorViewProj.proj = math::orthographic(
        -TERRAIN_HALF, TERRAIN_HALF,
        -TERRAIN_HALF, TERRAIN_HALF,
        0.1f, TERRAIN_SIZE);
#endif

    m_mvpDataBuffer.copyFrom(&m_data);
}

/* ========================================================================== */

#if ENABLE_SHADOW_MAPPING
const ImageBuffer& PFDSet::getShadowmap() const noexcept {
    return m_textures[(u32)Texture::Shadowmap]->getImageBuffer();
}
#endif

} // namespace vox::gfx