/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_set.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 16:12:13 by etran             #+#    #+#             */
/*   Updated: 2024/03/26 17:13:19 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mvp_set.h"
#include "device.h"
#include "game_state.h"
#include "matrix.h"
#include "maths.h"

#include <stdexcept>

#include "debug.h"

namespace vox::gfx {

void MVPSet::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    BufferMetadata bufferData{};
    bufferData.m_size = sizeof(MvpUbo);
    bufferData.m_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferData.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    m_mvpDataBuffer.init(device, std::move(bufferData));
    m_mvpDataBuffer.map(device);

    std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS, (u32)BindingIndex::ViewProj),
        _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS, (u32)BindingIndex::GameData),
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

void MVPSet::destroy(const Device& device) {
    m_mvpDataBuffer.unmap(device);
    m_mvpDataBuffer.destroy(device);
    vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);

    LDEBUG("MVP descriptor set destroyed");
}

/* ========================================================================== */

void MVPSet::fill(const Device& device) {
    VkDescriptorBufferInfo viewProjInfo{};
    viewProjInfo.buffer = m_mvpDataBuffer.getBuffer();
    viewProjInfo.offset = (VkDeviceSize)MvpUbo::Offset::ViewProj;
    viewProjInfo.range = sizeof(MvpUbo::m_viewProj);

    VkDescriptorBufferInfo gameDataInfo{};
    gameDataInfo.buffer = m_mvpDataBuffer.getBuffer();
    gameDataInfo.offset = (VkDeviceSize)MvpUbo::Offset::GameData;
    gameDataInfo.range = sizeof(MvpUbo::m_gameData);

    std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &viewProjInfo, (u32)BindingIndex::ViewProj),
        _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &gameDataInfo, (u32)BindingIndex::GameData),
    };
    vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);

    LDEBUG("MVP descriptor set filled");
}

void MVPSet::update(const game::GameState& state) {
    constexpr math::Vect3   UP_VEC = {0.0f, 1.0f, 0.0f};

    const math::Vect3&  position = state.getController().getPosition();
    const math::Vect3&  front = state.getController().getView();
    const math::Vect3   right = math::normalize(math::cross(front, UP_VEC));
    const math::Vect3   up = math::cross(right, front);

    m_data.m_viewProj.view = math::lookAt(position, front, up, right);

    static const f32    fovRadians = math::radians(70.0f);
    constexpr f32       aspectRatio = 1200.0f / 800.0f;
    constexpr f32       nearPlane = 0.1f;
    constexpr f32       farPlane = 500.0f;

    m_data.m_viewProj.proj = math::perspective(fovRadians, aspectRatio, nearPlane, farPlane);

    const f32 period = state.getElapsedTime() * 0.1f;
    m_data.m_gameData.sunPos = math::Vect2(std::cos(period), std::sin(period));

    m_mvpDataBuffer.copyFrom(&m_data);
}

} // namespace vox::gfx