/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:50:48 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:16:13 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_pipeline.h"
#include "device.h"
#include "input_handler.h"
#include "uniform_buffer_object.h"
#include "command_buffer.h"

#include "scene_texture_handler.h"
#include "scene_render_pass.h"
#include "scene_target.h"
#include "scene_descriptor_set.h"
#include "chunk_macros.h"

#include <stdexcept> // std::runtime_error
#include <chrono>

#if defined(__LINUX)
# define SCENE_VERTEX_PATH "shaders/scene.vertex.spv"
# define SCENE_FRAGMENT_PATH "shaders/scene.fragment.spv"
#else
# define SCENE_VERTEX_PATH "shaders/scene_vert.spv"
# define SCENE_FRAGMENT_PATH "shaders/scene_frag.spv"
#endif

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

ScenePipeline::ScenePipeline() {
	super::_render_pass = std::make_shared<SceneRenderPass>();
	super::_target = std::make_shared<SceneTarget>();
	super::_texture = std::make_shared<SceneTextureHandler>();
	super::_descriptor = std::make_shared<SceneDescriptorSet>();
}

void	ScenePipeline::init(
	scop::core::Device& device,
	RenderPassInfo& rp_info,
	TargetInfo& tar_info
) {
	super::_texture->init(device);
	super::_render_pass->init(device, rp_info);
	tar_info.render_pass = super::_render_pass;
	super::_target->init(device, tar_info);
	super::_descriptor->init(device);

	_ubo.init(
		device,
		sizeof(UniformBufferObject),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	_ubo.map(device.getLogicalDevice(), sizeof(UniformBufferObject));
}

void	ScenePipeline::assemble(
	scop::core::Device& device,
	VkGraphicsPipelineCreateInfo& info
) {
	/* SHADERS ================================================================= */
	VkShaderModule	vert_module =
		super::_createShaderModule(device, SCENE_VERTEX_PATH);
	VkShaderModule	frag_module =
		super::_createShaderModule(device, SCENE_FRAGMENT_PATH);

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
		throw std::runtime_error("failed to create scene gfx pipeline");
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

void	ScenePipeline::destroy(scop::core::Device& device) {
	_ubo.unmap(device.getLogicalDevice());
	_ubo.destroy(device.getLogicalDevice());

	super::destroy(device);
}

void	ScenePipeline::plugDescriptor(
	scop::core::Device& device,
	TextureHandlerPtr shadowmap,
	const scop::InputHandler& input
) {
	auto	scene_descriptors =
		std::dynamic_pointer_cast<SceneDescriptorSet>(super::_descriptor);
	scene_descriptors->plug(
		device,
		_ubo,
		super::_texture,
		shadowmap,
		input);
}

/**
 * @brief Record the drawing command of the pass.
*/
void	ScenePipeline::draw(
	VkPipelineLayout layout,
	CommandBuffer& command_buffer,
	const scop::InputHandler& input,
	int32_t image_index
) {
	_beginRenderPass(command_buffer, image_index);

	VkViewport	viewport{};
	viewport.width = static_cast<float>(_render_pass->getWidth());
	viewport.height = static_cast<float>(_render_pass->getHeight());
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(command_buffer.getBuffer(), 0, 1, &viewport);

	VkRect2D	scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = { _render_pass->getWidth(), _render_pass->getHeight() };
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

/**
 * @brief Update the adequate descriptors with the new ubo.
*/
void	ScenePipeline::update(const UniformBufferObject& ubo) noexcept {
	_ubo.copyFrom(&ubo, sizeof(UniformBufferObject));
}

/* ========================================================================== */

Buffer&	ScenePipeline::getUbo() noexcept {
	return _ubo;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	ScenePipeline::_beginRenderPass(
	CommandBuffer& command_buffer,
	int32_t image_index
) {
	std::array<VkClearValue, 2>	clear_values{};
	clear_values[0].color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
	clear_values[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo	render_pass{};
	render_pass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass.renderPass = _render_pass->getRenderPass();
	render_pass.framebuffer = _target->getFrameBuffers()[image_index];
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

} // namespace scop::gfx