/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 09:52:33 by etran             #+#    #+#             */
/*   Updated: 2023/08/15 19:19:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_pass.h"
#include "device.h"

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	RenderPass::destroy(scop::core::Device& device) {
	vkDestroyRenderPass(device.getLogicalDevice(), _render_pass, nullptr);
}

/* ========================================================================== */

VkRenderPass	RenderPass::getRenderPass() const noexcept {
	return _render_pass;
}

uint32_t	RenderPass::getWidth() const noexcept {
	return _width;
}

uint32_t	RenderPass::getHeight() const noexcept {
	return _height;
}

} // namespace scop::gfx