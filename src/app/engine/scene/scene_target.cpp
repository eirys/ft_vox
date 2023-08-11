/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_target.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 16:20:26 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 16:20:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_target.h"
#include "device.h"

#include <stdexcept> // std::runtime_error

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Create frame buffers wrapping each swap chain image view.
*/
void	SceneTarget::init(
	Device& device,
	const TargetInfo& info
) {
	const auto& attachments = info.fb_attachments;

	super::_frame_buffers.resize(attachments.size());
	for (std::size_t i = 0; i < attachments.size(); ++i) {
		// std::array<VkImageView, 3>	attachments = {
		// 	_color_image.getView(),
		// 	_depth_image.getView(),
		// 	_image_views[i]
		// };
		VkFramebufferCreateInfo	create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		create_info.renderPass = info.render_pass;
		create_info.attachmentCount = static_cast<uint32_t>(attachments[i].size());
		create_info.pAttachments = attachments[i].data();
		create_info.width = info.width;
		create_info.height = info.height;
		create_info.layers = 1;

		if (vkCreateFramebuffer(device.getLogicalDevice(), &create_info, nullptr, &(super::_frame_buffers[i])) != VK_SUCCESS) {
			throw std::runtime_error("failed to create frame buffer");
		}
	}
}

void	SceneTarget::destroy(Device& device) {
	for (std::size_t i = 0; i < super::_frame_buffers.size(); ++i) {
		vkDestroyFramebuffer(
			device.getLogicalDevice(),
			super::_frame_buffers[i],
			nullptr
		);
	}
}

} // namespace scop::graphics