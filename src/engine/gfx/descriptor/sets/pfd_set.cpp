/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pfd_set.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:12:13 by etran             #+#    #+#             */
/*   Updated: 2024/05/31 02:31:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pfd_set.h"
#include "device.h"
#include "game_state.h"
#include "matrix.h"
#include "maths.h"

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
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS, (u32)BindingIndex::ViewProj),
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS_FS, (u32)BindingIndex::GameData),
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
    VkDescriptorBufferInfo viewProjInfo{};
    viewProjInfo.buffer = m_mvpDataBuffer.getBuffer();
    viewProjInfo.offset = (VkDeviceSize)PFDUbo::Offset::ViewProj;
    viewProjInfo.range = sizeof(PFDUbo::m_viewProj);

    VkDescriptorBufferInfo gameDataInfo{};
    gameDataInfo.buffer = m_mvpDataBuffer.getBuffer();
    gameDataInfo.offset = (VkDeviceSize)PFDUbo::Offset::GameData;
    gameDataInfo.range = sizeof(PFDUbo::m_gameData);

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &viewProjInfo, (u32)BindingIndex::ViewProj),
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &gameDataInfo, (u32)BindingIndex::GameData),
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("PFD descriptor set filled");
}

void PFDSet::update(const game::GameState& state) {
    const ui::Camera&   camera = state.getController().getCamera();
    static const f32    fovRadians = math::radians(camera.m_fov);

    m_data.m_viewProj.view = math::lookAt(camera.m_position, camera.m_front, camera.m_up, camera.m_right);
    m_data.m_viewProj.proj = math::perspective(
        fovRadians,
        ui::Camera::ASPECT_RATIO,
        ui::Camera::NEAR_PLANE,
        ui::Camera::FAR_PLANE);

    constexpr math::Vect3 SUN_COLOR = {1.0f, 1.0f, 0.33f};
    constexpr math::Vect3 MOON_COLOR = {0.5f, 0.5f, 0.5f};

    m_data.m_gameData.sunPos = state.getSunPos().xy;
    m_data.m_gameData.skyHue = math::lerp(SUN_COLOR, MOON_COLOR, std::max(0.0f, state.getSunPos().y));

    m_mvpDataBuffer.copyFrom(&m_data);
}

} // namespace vox::gfx