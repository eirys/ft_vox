/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_constant.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 10:49:11 by etran             #+#    #+#             */
/*   Updated: 2024/06/28 22:18:09 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_constant.h"
#include "pipeline_layout.h"
#include "icommand_buffer.h"
#include "game_state.h"
#include "maths.h"
#include "debug.h"

#include <cassert>

namespace vox::gfx {

/* PUSH CONSTANT ============================================================ */

const std::vector<VkPushConstantRange>& PushConstant::getRanges() const noexcept {
    return m_ranges;
}

/* CAMERA PUSH CONSTANT ===================================================== */

CameraPushConstant::CameraPushConstant() {
    m_ranges.resize((u32)Objects::Count);

    m_ranges[(u32)Objects::Data].offset = offsetof(CameraPushConstant::Data, m_view);
    m_ranges[(u32)Objects::Data].size = sizeof(CameraPushConstant::Data);
    m_ranges[(u32)Objects::Data].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    LDEBUG("Camera push constant created.");
}

void CameraPushConstant::update() noexcept {
    const game::Camera&   camera = game::GameState::getCamera();

    m_data.m_view = math::lookAt(
        camera.m_position,
        camera.m_directions.front,
        camera.m_directions.up,
        camera.m_directions.right);
    m_data.m_proj = math::perspective(
        math::radians(camera.getSettings().fov),
        camera.ASPECT_RATIO,
        camera.getSettings().nearPlane,
        camera.getSettings().farPlane);
}

void CameraPushConstant::bind(const ICommandBuffer* cmdBuffer, const PipelineLayout& layout) const {
    for (u32 i = 0; i < (u32)Objects::Count; ++i) {
        vkCmdPushConstants(
            cmdBuffer->getBuffer(),
            layout.getLayout(),
            m_ranges[i].stageFlags,
            m_ranges[i].offset,
            m_ranges[i].size,
            getObject(i));
    }
}

VkPushConstantRange CameraPushConstant::getRange(const u32 index) const noexcept {
    return m_ranges[index];
}

const void* CameraPushConstant::getObject(const u32 index) const noexcept {
    switch ((Objects)index) {
        case Objects::Data: return (void*)&m_data;
        default:
            assert(false);
    }
}

} // namespace vox::gfx
