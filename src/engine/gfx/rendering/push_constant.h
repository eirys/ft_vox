/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_constant.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:21:33 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:35:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "matrix.h"
#include "types.h"
#include "vox_decl.h"

namespace game {
class GameState;
}

namespace vox::gfx {

class PipelineLayout;
class ICommandBuffer;

class PushConstant {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~PushConstant() = default;

    virtual void    update(const game::GameState& gameState) noexcept = 0;

    virtual void    bind(const ICommandBuffer* cmdBuffer, const PipelineLayout& layout) const = 0;

    virtual VkPushConstantRange getRange(const u32 index) const noexcept = 0;
    virtual const void*         getObject(const u32 index) const noexcept = 0;

    const std::vector<VkPushConstantRange>& getRanges() const noexcept;

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::vector<VkPushConstantRange>    m_ranges;

}; // class PushConstant

// ----------------------------------------------------------------------------

class CameraPushConstant final: public PushConstant {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Objects: u32 {
        Data,

        Count
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    CameraPushConstant();

    void    update(const game::GameState& gameState) noexcept override;
    void    bind(const ICommandBuffer* cmdBuffer, const PipelineLayout& layout) const override;

    const void*         getObject(const u32 index) const noexcept override;
    VkPushConstantRange getRange(const u32 index) const noexcept override;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    struct Data {
        math::Mat4  m_view;
        math::Mat4  m_proj;
    }   m_data;

}; // class CameraPushConstant

} // namespace vox::gfx
