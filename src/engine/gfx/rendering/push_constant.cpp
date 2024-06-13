/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_constant.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 10:49:11 by etran             #+#    #+#             */
/*   Updated: 2024/06/13 15:32:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_constant.h"
#include "game_state.h"
#include "maths.h"

#include <cassert>

namespace vox::gfx {

/* PUSH CONSTANT ============================================================ */

const std::vector<VkPushConstantRange>& PushConstant::getRanges() const noexcept {
    return m_ranges;
}

/* CAMERA PUSH CONSTANT ===================================================== */

MainPushConstant::MainPushConstant() {
    m_ranges.resize((u32)Objects::Count);

    m_ranges[(u32)Objects::Camera].offset = Camera::OFFSET;
    m_ranges[(u32)Objects::Camera].size = sizeof(MainPushConstant::Camera);
    m_ranges[(u32)Objects::Camera].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
}

void MainPushConstant::update(const game::GameState& gameState) noexcept {
    const ui::Camera&   camera = gameState.getController().getCamera();

    m_camera.m_view = math::lookAt(camera.m_position, camera.m_front, camera.m_up, camera.m_right);
    m_camera.m_proj = math::perspective(
        math::radians(camera.m_fov),
        ui::Camera::ASPECT_RATIO,
        ui::Camera::NEAR_PLANE,
        ui::Camera::FAR_PLANE);
}

void* MainPushConstant::getObject(const u32 index) const noexcept {
    switch ((Objects)index) {
        case Objects::Camera: return (void*)&m_camera;
        default:
            assert(false);
    }
}

VkPushConstantRange MainPushConstant::getRange(const u32 index) const noexcept {
    return m_ranges[index];
}

u32 MainPushConstant::getObjectCount() const noexcept {
    return (u32)Objects::Count;
}

} // namespace vox::gfx
