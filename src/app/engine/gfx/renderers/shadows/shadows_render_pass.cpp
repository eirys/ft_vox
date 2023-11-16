/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_render_pass.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 10:58:14 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:22:31 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shadows_render_pass.h"
#include "device.h"

#include <stdexcept> // std::runtime_error
#include <array> // std::array
#include <cassert> // static_assert

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	ShadowsRenderPass::init(
	scop::core::Device& device,
	const RenderPassInfo& rp_info
) {
	_createRenderPass(device, rp_info);
	_importResources(rp_info);
}

void	ShadowsRenderPass::destroy(scop::core::Device& device) {
	super::destroy(device);
}

/**
 * @brief Should never be updated.
*/
void	ShadowsRenderPass::updateResources(
	scop::core::Device& device,
	const RenderPassInfo& rp_info
) {
	assert(false);
	(void)device;
	(void)rp_info;
}

/* ========================================================================== */

const ImageBuffer&	ShadowsRenderPass::getDepthResource() const noexcept {
	return _depth_image;
}

ImageBuffer&	ShadowsRenderPass::getDepthResource() noexcept {
	return _depth_image;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	ShadowsRenderPass::_createRenderPass(
	scop::core::Device& device,
	const RenderPassInfo& rp_info
) {
	// Attachments
	VkAttachmentDescription	depth_attachment{};
	depth_attachment.format = rp_info.depth_format;
	depth_attachment.samples = rp_info.depth_samples;
	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

	// Subpass
	VkAttachmentReference	depth_ref{};
	depth_ref.attachment = 0;
	depth_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription	subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 0;
	subpass.pDepthStencilAttachment = &depth_ref;

	// Dependency
	std::array<VkSubpassDependency, 2>	dependencies;
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkRenderPassCreateInfo	render_pass{};
	render_pass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass.attachmentCount = 1;
	render_pass.pAttachments = &depth_attachment;
	render_pass.subpassCount = 1;
	render_pass.pSubpasses = &subpass;
	render_pass.dependencyCount = static_cast<uint32_t>(dependencies.size());
	render_pass.pDependencies = dependencies.data();

	if (vkCreateRenderPass(device.getLogicalDevice(), &render_pass, nullptr, &(super::_render_pass)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass");
	}
}

void	ShadowsRenderPass::_importResources(const RenderPassInfo& rp_info) {
	_depth_image = *rp_info.texture_buffer;
	super::_width = rp_info.width;
	super::_height = rp_info.height;
}

} // namespace scop::gfx