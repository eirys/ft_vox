/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_set.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:38:00 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 15:46:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "mvp_ubo.h"

#include "device.h"
#include "buffer.h"
#include <stdexcept>

namespace vox::gfx {

// Placeholder
class MVPSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Field: u32 {
        MvpMatrix = 0,

        First = MvpMatrix,
        Last = MvpMatrix
    };

    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = DescriptorSet;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    MVPSet(): super(DescriptorSetIndex::Mvp) {}

    void init(const Device& device) override {
        BufferMetadata metadata{};
        metadata.m_size = sizeof(ubo::MvpUbo);
        metadata.m_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        metadata.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        m_buffer.init(device, std::move(metadata));
        m_buffer.map(device);
        m_buffer.copyFrom(&m_data);

        std::array<VkDescriptorSetLayoutBinding, FIELD_COUNT> bindings = {
            _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS, (u32)Field::MvpMatrix)
        };

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = FIELD_COUNT;
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    void fill(const Device& device, const GameState& state) override {
        (void)state;

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = m_buffer.getBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(ubo::MvpUbo);

        std::array<VkWriteDescriptorSet, FIELD_COUNT> descriptorWrites = {
            _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &bufferInfo, (u32)Field::MvpMatrix)
        };

        vkUpdateDescriptorSets(device.getDevice(), FIELD_COUNT, descriptorWrites.data(), 0, nullptr);
    }

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 FIELD_COUNT = enumSize<Field>();

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Buffer      m_buffer;
    ubo::MvpUbo m_data;

};

} // namespace vox::gfx