/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idescriptor_set.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 20:41:22 by etran             #+#    #+#             */
/*   Updated: 2024/03/15 16:39:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include <array>

#include "types.h"
#include "enum.h"
#include "descriptor_decl.h"

namespace game {
class GameState;
}

namespace vox::gfx {

class Device;
class ICommandBuffer;

class IDescriptorSet {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using PoolSizes = std::array<VkDescriptorPoolSize, DESCRIPTOR_TYPE_COUNT>;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~IDescriptorSet() = default;

    /* ====================================================================== */

    virtual void    init(const Device& device, const ICommandBuffer* cmdBuffer) = 0;
    virtual void    destroy(const Device& device) = 0;

    virtual void    fill(const Device& device) = 0;
    virtual void    setDescriptorSet(const VkDescriptorSet set) noexcept = 0;

    /* ====================================================================== */

    virtual const PoolSizes&        getSizes() const noexcept = 0;
    virtual VkDescriptorSetLayout   getLayout() const noexcept = 0;
    virtual VkDescriptorSet         getSet() const noexcept = 0;
    virtual DescriptorSetIndex      getSetIndex() const noexcept = 0;

protected:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    IDescriptorSet() = default;

    IDescriptorSet(IDescriptorSet&& other) = delete;
    IDescriptorSet(const IDescriptorSet& other) = delete;
    IDescriptorSet& operator=(IDescriptorSet&& other) = delete;
    IDescriptorSet& operator=(const IDescriptorSet& other) = delete;

}; // class IDescriptorSet

} // namespace vox::gfx
