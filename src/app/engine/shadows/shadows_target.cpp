/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_target.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 15:00:10 by etran             #+#    #+#             */
/*   Updated: 2023/08/17 15:00:10 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shadows_target.h"
#include "device.h"

#include "shadows_render_pass.h"

#include <stdexcept> // std::runtime_error

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	ShadowsTarget::init(
	Device& device,
	const Target::TargetInfo& tar_info
) {
	const auto& render_pass =
		std::dynamic_pointer_cast<ShadowsRenderPass>(tar_info.render_pass);
	VkImageView view = render_pass->getDepthResource().getView();

	super::_frame_buffers.resize(1);

	VkFramebufferCreateInfo	fb_info{};
	fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fb_info.renderPass = render_pass->getRenderPass();
	fb_info.attachmentCount = 1;
	fb_info.pAttachments = &view;
	fb_info.width = tar_info.width;
	fb_info.height = tar_info.height;
	fb_info.layers = 1;

	if (vkCreateFramebuffer(device.getLogicalDevice(), &fb_info, nullptr, &(super::_frame_buffers[0])) != VK_SUCCESS) {
		throw std::runtime_error("failed to create frame buffer");
	}
}

/**
 * @brief So far, not useful
*/
void	ShadowsTarget::update(
	Device& device,
	const Target::TargetInfo& info
) {}

} // namespace scop::graphics