/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_constant.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:21:33 by etran             #+#    #+#             */
/*   Updated: 2024/06/13 15:32:54 by etran            ###   ########.fr       */
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

class PushConstant {
public:
    virtual ~PushConstant() = default;

    virtual void    update(const game::GameState& gameState) noexcept = 0;
    virtual void*   getObject(const u32 index) const noexcept = 0;
    virtual u32     getObjectCount() const noexcept = 0;
    virtual VkPushConstantRange getRange(const u32 index) const noexcept = 0;

    const std::vector<VkPushConstantRange>& getRanges() const noexcept;

protected:
    std::vector<VkPushConstantRange>    m_ranges;
};

class MainPushConstant final: public PushConstant {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class Objects: u32 {
        Camera,

        Count
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    MainPushConstant();

    void    update(const game::GameState& gameState) noexcept override;
    void*   getObject(const u32 index) const noexcept override;
    u32     getObjectCount() const noexcept override;
    VkPushConstantRange getRange(const u32 index) const noexcept override;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    struct Camera {
        static constexpr u32 OFFSET = 0;

        math::Mat4  m_view;
        math::Mat4  m_proj;
    }   m_camera;

};

} // namespace vox::gfx
