/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 21:26:12 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 02:23:41 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_pass.h"
#include "icommand_buffer.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void RenderPass::end(const ICommandBuffer* cmdBuffer) const {
    vkCmdEndRenderPass(cmdBuffer->getBuffer());
}

VkRenderPass RenderPass::getRenderPass() const noexcept {
    return m_vkRenderPass;
}

u32 RenderPass::getWidth() const noexcept {
    return m_width;
}

u32 RenderPass::getHeight() const noexcept {
    return m_height;
}

} // namespace vox::gfx