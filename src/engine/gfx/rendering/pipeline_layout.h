/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_layout.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:38:03 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 19:27:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include "types.h"
#include "push_constant.h"

namespace vox::gfx {

class Device;
class ICommandBuffer;
class IDescriptorSet;

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
        const std::vector<IDescriptorSet*>& setLayouts,
        PushConstant* pushConstant);
    void destroy(const Device& device);

    void updatePushConstant(const game::GameState& gameState) const;
    void bindPushConstantRange(const ICommandBuffer* commandBuffer) const;

    VkPipelineLayout                    getLayout() const noexcept;
    const std::vector<VkDescriptorSet>& getSets() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::vector<VkDescriptorSet>    m_descriptorSets;
    VkPipelineLayout                m_layout = VK_NULL_HANDLE;
    PushConstant*                   m_pushConstant = nullptr;

}; // class PipelineLayout

} // namespace vox::gfx