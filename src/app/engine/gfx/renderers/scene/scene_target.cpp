/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_target.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 16:20:26 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:12:55 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_target.h"
#include "device.h"
#include "swap_chain.h"
#include "scene_render_pass.h"

#include <array> // std::array
#include <stdexcept> // std::runtime_error

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Create frame buffers wrapping each swap chain image view.
*/
void	SceneTarget::init(
	scop::core::Device& device,
	const TargetInfo& tar_info
) {
	const auto& image_views = tar_info.swap_chain->getImageViews();
	auto render_pass =
		std::dynamic_pointer_cast<SceneRenderPass>(tar_info.render_pass);

	super::_frame_buffers.resize(image_views.size());

	std::array<VkImageView, 3>	attachments = {
		render_pass->getColorResource().getView(),
		render_pass->getDepthResource().getView() };

	for (std::size_t i = 0; i < image_views.size(); ++i) {
		VkFramebufferCreateInfo	framebuffer{};
		attachments[2] = image_views[i];

		framebuffer.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer.renderPass = render_pass->getRenderPass();
		framebuffer.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebuffer.pAttachments = attachments.data();
		framebuffer.width = tar_info.swap_chain->getExtent().width;
		framebuffer.height = tar_info.swap_chain->getExtent().height;
		framebuffer.layers = 1;

		if (vkCreateFramebuffer(device.getLogicalDevice(), &framebuffer, nullptr, super::_frame_buffers.data() + i) != VK_SUCCESS) {
			throw std::runtime_error("failed to create frame buffer");
		}
	}
}

void	SceneTarget::update(
	scop::core::Device& device,
	const TargetInfo& info
) {
	destroy(device);
	init(device, info);
}

} // namespace scop::gfx