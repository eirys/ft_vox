/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_render_pass.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:44:57 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 22:50:18 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_render_pass.h"
#include "device.h"
#include "swap_chain.h"

#include <stdexcept> // std::runtime_error
#include <array> // std::array

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	SceneRenderPass::init(
	Device& device,
	const RenderPass::RenderPassInfo& rp_info
) {
	_createRenderPass(device, rp_info);
	_createResources(device, rp_info);
}

void	SceneRenderPass::destroy(Device& device) {
	_destroyResources(device);
	super::destroy(device);
}

void	SceneRenderPass::updateResources(
	Device& device,
	const RenderPass::RenderPassInfo& rp_info
) {
	_destroyResources(device);
	_createResources(device, rp_info);
}

/* ========================================================================== */

ImageBuffer&	SceneRenderPass::getColorResource() noexcept {
	return _color_image;
}

const ImageBuffer&	SceneRenderPass::getColorResource() const noexcept {
	return _color_image;
}

ImageBuffer&	SceneRenderPass::getDepthResource() noexcept {
	return _depth_image;
}

const ImageBuffer&	SceneRenderPass::getDepthResource() const noexcept {
	return _depth_image;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	SceneRenderPass::_createRenderPass(
	Device& device,
	const RenderPass::RenderPassInfo& rp_info
) {
	// Attachments
	VkAttachmentDescription	color_attachment{};
	color_attachment.format = rp_info.color_format;
	color_attachment.samples = rp_info.color_samples;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription	depth_attachment{};
	depth_attachment.format = rp_info.depth_format;
	depth_attachment.samples = rp_info.depth_samples;
	depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription	color_attachment_resolve{};
	color_attachment_resolve.format = rp_info.color_format;
	color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	std::array<VkAttachmentDescription, 3>	attachments = {
		color_attachment,
		depth_attachment,
		color_attachment_resolve };

	// Subpass
	VkAttachmentReference	color_ref{};
	VkAttachmentReference	depth_ref{};
	VkAttachmentReference	color_resolve_ref{};
	color_ref.attachment = 0;
	color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	depth_ref.attachment = 1;
	depth_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	color_resolve_ref.attachment = 2;
	color_resolve_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription	subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_ref;
	subpass.pDepthStencilAttachment = &depth_ref;
	subpass.pResolveAttachments = &color_resolve_ref;

	// Dependency
	VkSubpassDependency	dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
		VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
		VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstAccessMask =
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
		VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo	render_pass{};
	render_pass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass.attachmentCount = static_cast<uint32_t>(attachments.size());
	render_pass.pAttachments = attachments.data();
	render_pass.subpassCount = 1;
	render_pass.pSubpasses = &subpass;
	render_pass.dependencyCount = 1;
	render_pass.pDependencies = &dependency;

	if (vkCreateRenderPass(device.getLogicalDevice(), &render_pass, nullptr, &(super::_render_pass)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass");
	}
}

void	SceneRenderPass::_createResources(
	Device& device,
	const RenderPass::RenderPassInfo& rp_info
) {
	super::_width = rp_info.width;
	super::_height = rp_info.height;

	_color_image.initImage(
		device,
		super::_width,
		super::_height,
		rp_info.color_format,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		device.getMsaaSamples());
	_color_image.initView(
		device,
		rp_info.color_format,
		VK_IMAGE_ASPECT_COLOR_BIT);

	_depth_image.initImage(
		device,
		super::_width,
		super::_height,
		rp_info.depth_format,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		device.getMsaaSamples());
	_depth_image.initView(
		device,
		rp_info.depth_format,
		VK_IMAGE_ASPECT_DEPTH_BIT);
}

void	SceneRenderPass::_destroyResources(Device& device) {
	_color_image.destroy(device);
	_depth_image.destroy(device);
}

} // namespace scop::graphics