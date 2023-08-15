/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:50:48 by etran             #+#    #+#             */
/*   Updated: 2023/08/15 19:43:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_pipeline.h"
#include "device.h"
#include "input_handler.h"
#include "uniform_buffer_object.h"
#include "command_buffer.h"

#include "descriptor_set.h"

#include "scene_texture_handler.h"
#include "scene_render_pass.h"
#include "scene_target.h"

#include <stdexcept> // std::runtime_error

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	ScenePipeline::init(
	Device& device,
	const RenderPass::RenderPassInfo& rp_info,
	const RenderPass::ResourcesInfo& res_info,
	Target::TargetInfo& tar_info,
	const std::vector<Texture>& textures,
	VkGraphicsPipelineCreateInfo& info
) {
	_createRenderPass(device, rp_info, res_info);
	_createPipeline(device, info);
	_createTarget(device, tar_info);
	_createTextureHandler(device, textures);
	_createDescriptor(device);
}

/**
 * @brief Record the drawing command of the pass.
*/
void	ScenePipeline::draw(
	Device& device,
	VkPipelineLayout layout,
	CommandBuffer& command_buffer,
	InputHandler& input,
	int32_t image_index
) {
	command_buffer.reset();
	command_buffer.begin(0);

	// Define what corresponds to 'clear color'
	std::array<VkClearValue, 2>	clear_values{};
	clear_values[0].color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
	clear_values[1].depthStencil = { 1.0f, 0 };

	// Spectify to render pass how to handle the command buffer,
	// and which framebuffer to render to
	VkRenderPassBeginInfo	render_pass_info{};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_info.renderPass = _render_pass->getRenderPass();
	render_pass_info.framebuffer = _target->getFrameBuffers()[image_index];
	render_pass_info.renderArea.offset = { 0, 0 };
	render_pass_info.renderArea.extent = {
		_render_pass->getWidth(),
		_render_pass->getHeight() };
	render_pass_info.clearValueCount =
		static_cast<uint32_t>(clear_values.size());
	render_pass_info.pClearValues = clear_values.data();

	// Begin rp and bind _pipeline
	vkCmdBeginRenderPass(
		command_buffer.getBuffer(),
		&render_pass_info,
		VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(
		command_buffer.getBuffer(),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		super::_pipeline);

	// Set viewport and scissors
	VkViewport	viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_render_pass->getWidth());
	viewport.height = static_cast<float>(_render_pass->getHeight());
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(command_buffer.getBuffer(), 0, 1, &viewport);

	VkRect2D	scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = { _render_pass->getWidth(), _render_pass->getHeight() };
	vkCmdSetScissor(command_buffer.getBuffer(), 0, 1, &scissor);

	// Bind vertex buffer && index buffer
	VkBuffer		vertex_buffers[] = { input.getVertexBuffer().getBuffer() };
	VkDeviceSize	offsets[] = { 0 };
	vkCmdBindVertexBuffers(
		command_buffer.getBuffer(),
		0,
		1, vertex_buffers,
		offsets);
	vkCmdBindIndexBuffer(
		command_buffer.getBuffer(),
		input.getIndexBuffer().getBuffer(),
		0,
		VK_INDEX_TYPE_UINT32);

	// Bind descriptor sets
	VkDescriptorSet	descriptor_set ;//= _descriptor_pool.getSet();
	vkCmdBindDescriptorSets(
		command_buffer.getBuffer(),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		layout,
		0,
		1, &descriptor_set,
		0, nullptr);

	// Issue draw command
	vkCmdDrawIndexed(
		command_buffer.getBuffer(),
		static_cast<uint32_t>(input.getIndicesCount()),
		1, 0, 0, 0);

	// Stop the render target work
	vkCmdEndRenderPass(command_buffer.getBuffer());
	command_buffer.end(device, false);
}

/**
 * @brief Update the adequate descriptors with the new ubo.
*/
void	ScenePipeline::update(const ::scop::UniformBufferObject& ubo) noexcept {
	_descriptor->update(ubo);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	ScenePipeline::_createPipeline(
	Device& device,
	VkGraphicsPipelineCreateInfo& info
) {
	/* SHADERS ================================================================= */
	VkShaderModule	vert_module =
		super::_createShaderModule(device, "shaders/scene_vert.spv");
	VkShaderModule	frag_module =
		super::_createShaderModule(device, "shaders/scene_frag.spv");

	VkPipelineShaderStageCreateInfo	vert_info{};
	vert_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vert_info.module = vert_module;
	vert_info.pName = "main";

	VkPipelineShaderStageCreateInfo	frag_info{};
	frag_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_info.module = frag_module;
	frag_info.pName = "main";

	std::array<VkPipelineShaderStageCreateInfo, 2>	shader_stages = {
		vert_info,
		frag_info };

	info.stageCount = static_cast<uint32_t>(shader_stages.size());
	info.pStages = shader_stages.data();
	info.renderPass = _render_pass->getRenderPass();

	if (vkCreateGraphicsPipelines(device.getLogicalDevice(), VK_NULL_HANDLE, 1, &info, nullptr, &(super::_pipeline)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics _pipeline");
	}

	vkDestroyShaderModule(
		device.getLogicalDevice(),
		vert_module,
		nullptr);
	vkDestroyShaderModule(
		device.getLogicalDevice(),
		frag_module,
		nullptr);
}

void	ScenePipeline::_createRenderPass(
	Device& device,
	const RenderPass::RenderPassInfo& rp_info,
	const RenderPass::ResourcesInfo& res_info
) {
	super::_render_pass.reset(new SceneRenderPass);
	super::_render_pass->init(device, rp_info, res_info);
}

void	ScenePipeline::_createTarget(
	Device& device,
	Target::TargetInfo& tar_info
) {
	super::_target.reset(new SceneTarget);
	tar_info.render_pass = super::_render_pass;
	super::_target->init(device, tar_info);
}

void	ScenePipeline::_createTextureHandler(
	Device& device,
	const std::vector<Texture>& textures
) {
	super::_texture.reset(new SceneTextureHandler);
	super::_texture->init(device, textures);
}

void	ScenePipeline::_createDescriptor(Device& device) {
	::scop::UniformBufferObject	ubo{};


	super::_descriptor->init(device, *super::_texture, ubo);
}

} // namespace scop::graphics