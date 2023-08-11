/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 09:52:33 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:09:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_pass.h"
#include "device.h"

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	RenderPass::destroy(Device& device) {
	vkDestroyRenderPass(device.getLogicalDevice(), _render_pass, nullptr);
}

/* ========================================================================== */

VkRenderPass	RenderPass::getRenderPass() const noexcept {
	return _render_pass;
}

} // namespace scop::graphics