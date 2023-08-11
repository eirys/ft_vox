/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:50:48 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:31:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_pipeline.h"
#include "device.h"
#include "input_handler.h"

#include "scene_texture_handler.h"
#include "scene_render_pass.h"

#include <stdexcept> // std::runtime_error

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	ScenePipeline::init(
	Device& device,
	CommandPool& pool,
	const RenderPass::RenderPassInfo& rp_info,
	const std::vector<Texture>& textures,
	VkGraphicsPipelineCreateInfo& info
) {
	_createRenderPass(device, rp_info);
	_createTextureHandler(device, pool, textures);
	_createPipeline(device, info);
}

void	ScenePipeline::record(
	Device& device,
	VkPipelineLayout layout,
	VkCommandBuffer command_buffer,
	InputHandler& input
) {

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
	const RenderPass::RenderPassInfo& rp_info
) {
	super::_render_pass.reset(new SceneRenderPass);
	super::_render_pass->init(device, rp_info);
}

void	ScenePipeline::_createTextureHandler(
	Device& device,
	CommandPool& pool,
	const std::vector<Texture>& textures
) {
	super::_texture_handler.reset(new SceneTextureHandler);
	super::_texture_handler->init(device, pool, textures);
}

} // namespace scop::graphics