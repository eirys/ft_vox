/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 21:26:12 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 21:49:18 by etran            ###   ########.fr       */
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

} // namespace vox::gfx