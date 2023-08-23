/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_pipeline.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 10:16:30 by etran             #+#    #+#             */
/*   Updated: 2023/08/22 22:12:30 by etran            ###   ########.fr       */
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

#include <stdexcept> // std::runtime_error

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

ShadowsPipeline::ShadowsPipeline() {
	super::_render_pass = std::make_shared<ShadowsRenderPass>();
	super::_target = std::make_shared<ShadowsTarget>();
	super::_texture = std::make_shared<ShadowsTextureHandler>();
}

void	ShadowsPipeline::init(
	Device& device,
	const RenderPass::RenderPassInfo& rp_info,
	Target::TargetInfo& tar_info,
	const std::vector<Texture>& textures,
	VkGraphicsPipelineCreateInfo& info
) {
	super::_render_pass->init(device, rp_info);
	_createPipeline(device, info);
	tar_info.render_pass = super::_render_pass;
	super::_target->init(device, tar_info);
	super::_texture->init(device, textures);
}

void	ShadowsPipeline::setDescriptor(DescriptorSetPtr desc_ptr) {
	using ImageInfo = DescriptorSet::ImageInfo;
	using UniformBufferObject = ::scop::UniformBufferObject;

	ImageInfo	texture{};
	texture.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	texture.binding = 4;
	texture.sampler = _texture->getTextureSampler();
	texture.view = _texture->getTextureBuffer().getView();
	texture.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	desc_ptr->addDescriptor(texture);

	super::setDescriptor(desc_ptr);
}

void	ShadowsPipeline::draw(
	VkPipelineLayout layout,
	CommandBuffer& command_buffer,
	InputHandler& input,
	int32_t image_index
) {
	std::array<VkClearValue, 1>	clear_values{};
	clear_values[0].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo	render_pass{};
	render_pass.renderPass = _render_pass->getRenderPass();
	render_pass.framebuffer = _target->getFrameBuffers()[0];
	render_pass.renderArea.extent.width = _render_pass->getWidth();
	render_pass.renderArea.extent.height = _render_pass->getHeight();
	render_pass.clearValueCount = static_cast<uint32_t>(clear_values.size());
	render_pass.pClearValues = clear_values.data();

	vkCmdBeginRenderPass(
		command_buffer.getBuffer(),
		&render_pass,
		VK_SUBPASS_CONTENTS_INLINE );

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

	vkCmdBindPipeline(
		command_buffer.getBuffer(),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		super::_pipeline );

	VkDescriptorSet	descriptor_set = super::_descriptor->getSet();
	vkCmdBindDescriptorSets(
		command_buffer.getBuffer(),
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		layout,
		0,
		1, &descriptor_set,
		0, nullptr);


	std::array<VkBuffer, 1>		vertex_buffers = { input.getVertexBuffer().getBuffer() };
	std::array<VkDeviceSize, 1>	offsets = { 0 };
	vkCmdBindVertexBuffers(
		command_buffer.getBuffer(),
		0,
		static_cast<uint32_t>(vertex_buffers.size()), vertex_buffers.data(),
		offsets.data() );
	vkCmdBindIndexBuffer(
		command_buffer.getBuffer(),
		input.getVertexBuffer().getBuffer(),
		0,
		VK_INDEX_TYPE_UINT32 );

	vkCmdDrawIndexed(
		command_buffer.getBuffer(),
		static_cast<uint32_t>(input.getIndicesCount()),
		1, 0, 0, 0);

	vkCmdEndRenderPass(command_buffer.getBuffer());
}

/**
 * @brief Shouldn't need update so far (same descriptor as scene).
*/
void	ShadowsPipeline::update(
	const ::scop::UniformBufferObject& ubo
) noexcept {}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	ShadowsPipeline::_createPipeline(
	Device& device,
	VkGraphicsPipelineCreateInfo& info
) {
	/* SHADERS ================================================================= */
	VkShaderModule	vert_module =
		super::_createShaderModule(device, "shaders\\shadow_vert.spv");

	VkPipelineShaderStageCreateInfo	vert_info{};
	vert_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vert_info.module = vert_module;
	vert_info.pName = "main";

	info.pStages = &vert_info;
	info.stageCount = 1;
	info.renderPass = _render_pass->getRenderPass();

	if (vkCreateGraphicsPipelines(device.getLogicalDevice(), VK_NULL_HANDLE, 1, &info, nullptr, &(super::_pipeline)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shadow graphics pipeline");
	}

	vkDestroyShaderModule(
		device.getLogicalDevice(),
		vert_module,
		nullptr);
}

} // namespace scop::graphics