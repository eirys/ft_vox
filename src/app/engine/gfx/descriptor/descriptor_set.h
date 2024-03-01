/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:16:09 by etran             #+#    #+#             */
/*   Updated: 2024/02/29 22:41:00 by etran            ###   ########.fr       */
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

    void    destroy(const Device& device) override;
    void    setDescriptorSet(const VkDescriptorSet set) noexcept override;

    /* ====================================================================== */

    const PoolSizes&        getSizes() const noexcept override;
    VkDescriptorSetLayout   getLayout() const noexcept override;
    VkDescriptorSet         getSet() const noexcept override;
    DescriptorSetIndex      getSetIndex() const noexcept override;

protected:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    DescriptorSet(const DescriptorSetIndex index);

    DescriptorSet() = delete;
    DescriptorSet(DescriptorSet&& other) = delete;
    DescriptorSet(const DescriptorSet& other) = delete;
    DescriptorSet& operator=(DescriptorSet&& other) = delete;
    DescriptorSet& operator=(const DescriptorSet& other) = delete;

    /* ====================================================================== */

    VkDescriptorSetLayoutBinding    _createLayoutBinding(
        const DescriptorTypeIndex typeIndex,
        const ShaderVisibility shaderStage,
        const u32 bindingIndex,
        const u32 count = 1);

    VkWriteDescriptorSet            _createWriteDescriptorSet(
        const DescriptorTypeIndex typeIndex,
        const void* descriptorInfo,
        const u32 bindingIndex,
        const u32 count = 1) const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    PoolSizes               m_poolSizes;
    VkDescriptorSetLayout   m_layout = VK_NULL_HANDLE;
    VkDescriptorSet         m_set = VK_NULL_HANDLE;

    DescriptorSetIndex      m_index;

}; // class DescriptorSet

} // namespace vox::gfx
