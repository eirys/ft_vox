/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:50:48 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:07:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_pipeline.h"
#include "device.h"
#include "swap_chain.h"

namespace scop::graphics {
/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	ScenePipeline::init(
	Device& device,
	VkGraphicsPipelineCreateInfo& info
) {
	_render_pass = std::dynamic_pointer_cast<SceneRenderPass>(render_pass);
	_createPipeline(device, info);
}

void	ScenePipeline::destroy(Device& device) {

}

void	ScenePipeline::record(
	Device& device,
	VkCommandBuffer command_buffer
) {

}

/* ========================================================================== */

RenderPassPtr	ScenePipeline::getRenderPass() const noexcept {
	return std::dynamic_pointer_cast<RenderPass>(_render_pass);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	ScenePipeline::_createPipeline(
	Device& device,
	VkGraphicsPipelineCreateInfo& info
) {
	/* SHADERS ================================================================= */
	std::vector<VkPipelineShaderStageCreateInfo>	shader_stages;
	_populateShaderStages(device, shader_stages);

	/* VERTEX INPUT ============================================================ */
	auto	attribute_descriptions = scop::Vertex::getSceneAttributeDescriptions();

	/* COLOR BLENDING ========================================================== */
	VkPipelineColorBlendAttachmentState	color_blend_attachment{};
	color_blend_attachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;
	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

	info.stageCount = static_cast<uint32_t>(
		shader_stages.size());
	info.pStages = shader_stages.data();
	info.pVertexInputState->vertexAttributeDescriptionCount =
		static_cast<uint32_t>(attribute_descriptions.size());
	info.pVertexInputState->pVertexAttributeDescriptions =
		attribute_descriptions.data();
	info.pColorBlendState->attachmentCount = 1;
	info.pColorBlendState->pAttachments = &color_blend_attachment;
	info.pRasterizationState->cullMode = VK_CULL_MODE_BACK_BIT;
	info.pMultisampleState->rasterizationSamples = _device.getMsaaSamples();
	info.pDepthStencilState->depthCompareOp = VK_COMPARE_OP_LESS;
	info.renderPass = _render_pass.getRenderPass();

	if (vkCreateGraphicsPipelines(_device.getLogicalDevice(), VK_NULL_HANDLE, 1, &info, nullptr, &Pipeline::_pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics _pipeline");
	}

	vkDestroyShaderModule(
		_device.getLogicalDevice(),
		frag_module,
		nullptr
	);
	vkDestroyShaderModule(
		_device.getLogicalDevice(),
		vert_module,
		nullptr
	);
}

void	ScenePipeline::_populateShaderStages(
	Device& device,
	std::vector<VkPipelineShaderStageCreateInfo>& shader_stages
) {
	shader_stages.reserve(2);
	VkShaderModule	vert_module = _createShaderModule("shaders/scene_vert.spv");
	VkShaderModule	frag_module = _createShaderModule("shaders/scene_frag.spv");

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

	shader_stages.emplace_back(vert_info);
	shader_stages.emplace_back(frag_info);
}

} // namespace scop::graphics