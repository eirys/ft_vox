/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_pipeline.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 10:16:30 by etran             #+#    #+#             */
/*   Updated: 2023/10/06 12:37:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shadows_pipeline.h"
#include "device.h"
#include "command_buffer.h"
#include "descriptor_set.h"
#include "input_handler.h"

#include "shadows_target.h"
#include "shadows_render_pass.h"
#include "shadows_texture_handler.h"
#include "shadows_descriptor_set.h"

#include "image_buffer.h"
#include "engine.h"
#include "chunk_macros.h"

#include <stdexcept> // std::runtime_error

#if defined(__LINUX)
# define SHADOW_VERTEX_PATH "shaders/shadow.vertex.spv"
#else
# define SHADOW_VERTEX_PATH "shaders/shadow_vert.spv"
#endif

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

ShadowsPipeline::ShadowsPipeline() {
	super::_render_pass = std::make_shared<ShadowsRenderPass>();
	super::_target = std::make_shared<ShadowsTarget>();
	super::_texture = std::make_shared<ShadowsTextureHandler>();
	super::_descriptor = std::make_shared<ShadowsDescriptorSet>();
}

void	ShadowsPipeline::init(
	Device& device,
	RenderPass::RenderPassInfo& rp_info,
	Target::TargetInfo& tar_info
) {
	super::_texture->init(device);
	rp_info.texture_buffer = &(super::_texture->getTextureBuffer());
	super::_render_pass->init(device, rp_info);
	tar_info.render_pass = super::_render_pass;
	super::_target->init(device, tar_info);
	super::_descriptor->init(device);
}

void	ShadowsPipeline::assemble(
	Device& device,
	VkGraphicsPipelineCreateInfo& info
) {
	/* SHADERS ================================================================= */
	VkShaderModule	vert_module =
		super::_createShaderModule(device, SHADOW_VERTEX_PATH);

	VkPipelineShaderStageCreateInfo	vert_info{};
	vert_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vert_info.module = vert_module;
	vert_info.pName = "main";

	info.pStages = &vert_info;
	info.stageCount = 1;
	info.renderPass = super::_render_pass->getRenderPass();

	if (vkCreateGraphicsPipelines(device.getLogicalDevice(), VK_NULL_HANDLE, 1, &info, nullptr, &(super::_pipeline)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shadow graphics pipeline");
	}

	vkDestroyShaderModule(
		device.getLogicalDevice(),
		vert_module,
		nullptr);
}

void	ShadowsPipeline::plugDescriptor(
	Device& device,
	Buffer& ubo,
	const InputHandler& input
) {
	auto	shadows_descriptor =
		std::dynamic_pointer_cast<ShadowsDescriptorSet>(super::_descriptor);
	shadows_descriptor->plug(device, ubo, input);
}

void	ShadowsPipeline::draw(
	VkPipelineLayout layout,
	CommandBuffer& command_buffer,
	const InputHandler& input,
	int32_t image_index
) {
	(void)image_index;
	_beginRenderPass(command_buffer);

	VkViewport	viewport{};
	viewport.width = static_cast<float>(_render_pass->getWidth());
	viewport.height = static_cast<float>(_render_pass->getHeight());
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(command_buffer.getBuffer(), 0, 1, &viewport);

	VkRect2D	scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent.width = _render_pass->getWidth();
	scissor.extent.height = _render_pass->getHeight();
	vkCmdSetScissor(command_buffer.getBuffer(), 0, 1, &scissor);

	std::array<VkDescriptorSet, 1>	descriptor_sets = { super::_descriptor->getSet() };

	vkCmdBindDescriptorSets(
		command_buffer.getBuffer(),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		layout,
		super::_descriptor->getSetIndex(),
		static_cast<uint32_t>(descriptor_sets.size()), descriptor_sets.data(),
		0, nullptr);
	vkCmdBindPipeline(
		command_buffer.getBuffer(),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		super::_pipeline);

	vkCmdDraw(
		command_buffer.getBuffer(),
		input.getVerticesCount(),
		input.getInstancesCount(), 0, 0);

	vkCmdEndRenderPass(command_buffer.getBuffer());
}

/* ========================================================================== */

void	ShadowsPipeline::_beginRenderPass(
	CommandBuffer& command_buffer,
	int32_t image_index
) {
	(void)image_index;

	std::array<VkClearValue, 1>	clear_values{};
	clear_values[0].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo	render_pass{};
	render_pass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass.renderPass = _render_pass->getRenderPass();
	render_pass.framebuffer = _target->getFrameBuffers()[0];
	render_pass.renderArea.offset = { 0, 0 };
	render_pass.renderArea.extent.width = _render_pass->getWidth();
	render_pass.renderArea.extent.height = _render_pass->getHeight();
	render_pass.clearValueCount = static_cast<uint32_t>(clear_values.size());
	render_pass.pClearValues = clear_values.data();

	vkCmdBeginRenderPass(
		command_buffer.getBuffer(),
		&render_pass,
		VK_SUBPASS_CONTENTS_INLINE);
}

} // namespace scop::graphics