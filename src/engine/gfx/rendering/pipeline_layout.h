/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_layout.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:38:03 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:34:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "types.h"

namespace vox::gfx {

class Device;
class ICommandBuffer;
class IDescriptorSet;
class PushConstant;

class PipelineLayout final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    PipelineLayout() = default;
    PipelineLayout(PipelineLayout&& other) = default;
    PipelineLayout& operator=(PipelineLayout&& other) = default;
    ~PipelineLayout() = default;

    PipelineLayout(const PipelineLayout& other) = delete;
    PipelineLayout& operator=(const PipelineLayout& other) = delete;

    /* ====================================================================== */

    void init(
        const Device& device,
        const std::vector<const IDescriptorSet*>& setLayouts,
        const PushConstant* pushConstant = nullptr);
    void destroy(const Device& device);

    VkPipelineLayout                    getLayout() const noexcept;
    const std::vector<VkDescriptorSet>& getSets() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::vector<VkDescriptorSet>    m_descriptorSets;
    VkPipelineLayout                m_layout = VK_NULL_HANDLE;

}; // class PipelineLayout

} // namespace vox::gfx
