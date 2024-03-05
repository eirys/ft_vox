/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 21:26:12 by etran             #+#    #+#             */
/*   Updated: 2024/03/05 10:26:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_pass.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

VkRenderPass RenderPass::getRenderPass() const noexcept {
    return m_vkRenderPass;
}

u32 RenderPass::getWidth() const noexcept {
    return m_width;
}

u32 RenderPass::getHeight() const noexcept {
    return m_height;
}

const std::vector<VkFramebuffer>& RenderPass::getTargets() const noexcept {
    return m_targets;
}

const std::vector<ImageBuffer>& RenderPass::getResources() const noexcept {
    return m_resources;
}

} // namespace vox::gfx