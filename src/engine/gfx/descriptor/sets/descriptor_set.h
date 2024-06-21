/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:16:09 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:26:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "idescriptor_set.h"

namespace vox::gfx {

/**
 * @brief Descriptor set handle.
*/
class DescriptorSet: public IDescriptorSet {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~DescriptorSet() = default;

    /* ====================================================================== */

    void    setDescriptorSet(const VkDescriptorSet set) noexcept override;

    /* ====================================================================== */

    const PoolSizes&        getSizes() const noexcept override;
    VkDescriptorSetLayout   getLayout() const noexcept override;
    VkDescriptorSet         getSet() const noexcept override;

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkDescriptorSetLayout           m_layout = VK_NULL_HANDLE;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    DescriptorSet() = default;

    DescriptorSet(DescriptorSet&& other) = delete;
    DescriptorSet(const DescriptorSet& other) = delete;
    DescriptorSet& operator=(DescriptorSet&& other) = delete;
    DescriptorSet& operator=(const DescriptorSet& other) = delete;

    /* ====================================================================== */

    VkDescriptorSetLayoutBinding    _createLayoutBinding(
        const DescriptorTypeIndex typeIndex,
        const ShaderVisibility shaderStage,
        const u32 bindingIndex);

    VkWriteDescriptorSet            _createWriteDescriptorSet(
        const DescriptorTypeIndex typeIndex,
        const VkDescriptorImageInfo& imageInfo,
        const u32 bindingIndex) const;

    VkWriteDescriptorSet            _createWriteDescriptorSet(
        const DescriptorTypeIndex typeIndex,
        const VkDescriptorBufferInfo& bufferInfo,
        const u32 bindingIndex) const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    PoolSizes               m_poolSizes{};
    VkDescriptorSet         m_set = VK_NULL_HANDLE;

}; // class DescriptorSet

} // namespace vox::gfx
