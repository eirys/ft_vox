/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_set.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:38:00 by etran             #+#    #+#             */
/*   Updated: 2024/03/11 14:07:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "mvp_ubo.h"

#include "device.h"
#include "buffer.h"
#include <stdexcept>
#include "debug.h"

namespace vox::gfx {

// Placeholder
class MVPSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
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

        std::array<VkDescriptorSetLayoutBinding, BINDING_COUNT> bindings = {
            _createLayoutBinding(DescriptorTypeIndex::UniformBuffer, ShaderVisibility::VS, (u32)BindingIndex::MvpMatrix)
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

    void fill(const Device& device, const GameState& state) override {
        (void)state;

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = m_buffer.getBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(ubo::MvpUbo);

        std::array<VkWriteDescriptorSet, BINDING_COUNT> descriptorWrites = {
            _createWriteDescriptorSet(DescriptorTypeIndex::UniformBuffer, &bufferInfo, (u32)BindingIndex::MvpMatrix)
        };

        vkUpdateDescriptorSets(device.getDevice(), BINDING_COUNT, descriptorWrites.data(), 0, nullptr);
        LDEBUG("MVP descriptor set filled");
    }

    void    destroy(const Device& device) override {
        m_buffer.unmap(device);
        m_buffer.destroy(device);
        vkDestroyDescriptorSetLayout(device.getDevice(), m_layout, nullptr);
        LDEBUG("MVP descriptor set destroyed");
    }

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 BINDING_COUNT = enumSize<BindingIndex>();

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Buffer      m_buffer;
    ubo::MvpUbo m_data;

};

} // namespace vox::gfx