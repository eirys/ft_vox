/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_target.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 16:20:26 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 22:51:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_target.h"
#include "device.h"
#include "scene_render_pass.h"

#include <stdexcept> // std::runtime_error
#include <memory>

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Create frame buffers wrapping each swap chain image view.
*/
void	SceneTarget::init(
	Device& device,
	const Target::TargetInfo& tar_info
) {
	const auto& image_views = tar_info.swap_views;
	const auto& render_pass =
		std::dynamic_pointer_cast<SceneRenderPass>(tar_info.render_pass);

	super::_frame_buffers.resize(image_views.size());

	for (std::size_t i = 0; i < image_views.size(); ++i) {
		std::array<VkImageView, 3>	attachments = {
			render_pass->getColorResource().getView(),
			render_pass->getDepthResource().getView(),
			image_views[i] };
		VkFramebufferCreateInfo	create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		create_info.renderPass = render_pass->getRenderPass();
		create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
		create_info.pAttachments = attachments.data();
		create_info.width = tar_info.width;
		create_info.height = tar_info.height;
		create_info.layers = 1;

		if (vkCreateFramebuffer(device.getLogicalDevice(), &create_info, nullptr, &(super::_frame_buffers[i])) != VK_SUCCESS) {
			throw std::runtime_error("failed to create frame buffer");
		}
	}
}

void	SceneTarget::update(
	Device& device,
	const Target::TargetInfo& info
) {
	destroy(device);
	init(device, info);
}

} // namespace scop::graphics