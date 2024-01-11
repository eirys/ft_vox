/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_manager.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 22:56:55 by etran             #+#    #+#             */
/*   Updated: 2024/01/08 15:18:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_manager.h"
#include "device.h"
#include "swap_chain.h"
#include "render_pass.h"
#include "target.h"
#include "input_handler.h"
#include "descriptor_set.h"

#include "culling_pipeline.h"
#include "scene_pipeline.h"
#include "shadows_pipeline.h"

#include "shadowmap_consts.h"

#include "debug.h"

#include <stdexcept>

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

PipelineManager::PipelineManager() {
	_graphics.scene = std::make_shared<ScenePipeline>();
	_graphics.shadows = std::make_shared<ShadowsPipeline>();
	_compute.culling = std::make_shared<CullingPipeline>();
}

void	PipelineManager::init(
	scop::core::Device& device,
	const scop::SwapChain& swap_chain
) {
	_createComputesPipelines(device);
	_createGraphicsPipelines(device, swap_chain);
	_createPipelineLayout(device);

	_assembleComputePipelines(device);
	_assembleGraphicsPipelines(device);

}

void	PipelineManager::destroy(scop::core::Device& device) {
	vkDestroyPipelineLayout(device.getLogicalDevice(), _pipeline_layout, nullptr);

	_graphics.scene->destroy(device);
	_graphics.shadows->destroy(device);
	_compute.culling->destroy(device);
}

/* ========================================================================== */

void	PipelineManager::plugDescriptors(
	scop::core::Device& device,
	InputHandler& input_handler
) {
	using ScenePipelinePtr = std::shared_ptr<ScenePipeline>;
	using ShadowsPipelinePtr = std::shared_ptr<ShadowsPipeline>;
	using CullingPipelinePtr = std::shared_ptr<CullingPipeline>;

	ScenePipelinePtr scene_pipeline = std::dynamic_pointer_cast<ScenePipeline>(_graphics.scene);
	ShadowsPipelinePtr shadows_pipeline = std::dynamic_pointer_cast<ShadowsPipeline>(_graphics.shadows);
	CullingPipelinePtr culling_pipeline = std::dynamic_pointer_cast<CullingPipeline>(_compute.culling);

	SCOP_DEBUG("Filling descriptor sets: scene");
	scene_pipeline->plugDescriptor(device, shadows_pipeline->getTextureHandler(), input_handler);
	SCOP_DEBUG("Done filling descriptor sets: scene");
	SCOP_DEBUG("Filling descriptor sets: shadows");
	shadows_pipeline->plugDescriptor(device, scene_pipeline->getUbo(), input_handler);
	SCOP_DEBUG("Done filling descriptor sets: shadows");
	SCOP_DEBUG("Filling descriptor sets: culling");
	culling_pipeline->plugDescriptor(device, input_handler);
	SCOP_DEBUG("Done filling descriptor sets: culling");
}

/* ========================================================================== */

VkPipelineLayout	PipelineManager::getPipelineLayout() const noexcept {
	return _pipeline_layout;
}

PipelineManager::GraphicsPipelinePtr	PipelineManager::getScenePipeline() const noexcept {
	return _graphics.scene;
}

PipelineManager::GraphicsPipelinePtr	PipelineManager::getShadowsPipeline() const noexcept {
	return _graphics.shadows;
}

PipelineManager::ComputePipelinePtr	PipelineManager::getCullingPipeline() const noexcept {
	return _compute.culling;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	PipelineManager::_createComputesPipelines(scop::core::Device& device) {
	_compute.culling->init(device);
}

/**
 * @brief Create graphics pipelines and their render passes/components.
*/
void	PipelineManager::_createGraphicsPipelines(
	scop::core::Device& device,
	const scop::SwapChain& swap_chain
) {
	gfx::RenderPassInfo	rp_info {
		.width = swap_chain.getExtent().width,
		.height = swap_chain.getExtent().height,
		.depth_format = swap_chain.findDepthFormat(device),
		.depth_samples = device.getMsaaSamples(),
		.color_format = swap_chain.getImageFormat(),
		.color_samples = device.getMsaaSamples() };
	gfx::TargetInfo	tar_info { .swap_chain = &swap_chain };

	_graphics.scene->init(
		device,
		rp_info,
		tar_info);

	rp_info.width = SHADOWMAP_SIZE;
	rp_info.height = SHADOWMAP_SIZE;
	rp_info.depth_format = SHADOWMAP_DEPTH_FORMAT;
	rp_info.depth_samples = VK_SAMPLE_COUNT_1_BIT;

	_graphics.shadows->init(
		device,
		rp_info,
		tar_info);
}

void	PipelineManager::_createPipelineLayout(scop::core::Device& device) {
	std::vector<VkDescriptorSetLayout>	layouts = {
		_graphics.scene->getDescriptor()->getLayout(),
		_graphics.shadows->getDescriptor()->getLayout(),
		_compute.culling->getDescriptor()->getLayout() };
	VkPipelineLayoutCreateInfo	pipeline_layout_info{};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(layouts.size());
	pipeline_layout_info.pSetLayouts = layouts.data();
	pipeline_layout_info.pushConstantRangeCount = 0;
	pipeline_layout_info.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(device.getLogicalDevice(), &pipeline_layout_info, nullptr, &_pipeline_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout");
	}
}

void	PipelineManager::_assembleComputePipelines(scop::core::Device& device) {
	VkComputePipelineCreateInfo	pipeline_info{};
	pipeline_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipeline_info.layout = _pipeline_layout;
	pipeline_info.flags = 0;

	SCOP_DEBUG("PipelineManager::_assembleComputePipelines: culling");
	_compute.culling->assemble(device, pipeline_info);
	SCOP_DEBUG("Culling pipeline assembled.");
}

void	PipelineManager::_assembleGraphicsPipelines(scop::core::Device& device) {
	/* INPUT FORMAT ============================================================ */
	VkPipelineVertexInputStateCreateInfo	vert_input{};

	vert_input.sType =
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	vert_input.vertexBindingDescriptionCount = 0;
	vert_input.pVertexBindingDescriptions = nullptr;
	vert_input.vertexAttributeDescriptionCount = 0;
	vert_input.pVertexAttributeDescriptions = nullptr;

	/* INPUT ASSEMBLY ========================================================== */
	VkPipelineInputAssemblyStateCreateInfo	input_assembly{};
	input_assembly.sType =
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	/* VIEWPORT ================================================================ */
	VkPipelineViewportStateCreateInfo	viewport_state{};
	viewport_state.sType =
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.scissorCount = 1;

	/* RASTERIZER ============================================================== */
	VkPipelineRasterizationStateCreateInfo	rasterizing{};
	rasterizing.sType =
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizing.depthClampEnable = VK_FALSE;
	rasterizing.rasterizerDiscardEnable = VK_FALSE;
	rasterizing.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizing.lineWidth = 1.0f;
	rasterizing.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizing.depthBiasEnable = VK_FALSE;
	rasterizing.depthBiasConstantFactor = 0.0f;
	rasterizing.depthBiasClamp = 0.0f;
	rasterizing.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizing.depthBiasSlopeFactor = 0.0f;

	/* MULTISAMPLING =========================================================== */
	VkPipelineMultisampleStateCreateInfo	multisampling{};
	multisampling.sType =
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;
	multisampling.rasterizationSamples = device.getMsaaSamples();

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

	VkPipelineColorBlendStateCreateInfo	color_blending{};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY;
	color_blending.blendConstants[0] = 0.0f;
	color_blending.blendConstants[1] = 0.0f;
	color_blending.blendConstants[2] = 0.0f;
	color_blending.blendConstants[3] = 0.0f;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;

	/* DEPTH STENCIL =========================================================== */
	VkPipelineDepthStencilStateCreateInfo	depth_stencil{};
	depth_stencil.sType =
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil.depthTestEnable = VK_TRUE;			// fragment depth compared to depth buffer enabled
	depth_stencil.depthWriteEnable = VK_TRUE;			// if test passed, new depth saved in buffer enabled
	depth_stencil.depthBoundsTestEnable = VK_FALSE;		// unused. specifies min/max depth bounds
	depth_stencil.minDepthBounds = 0.0f;
	depth_stencil.maxDepthBounds = 1.0f;
	depth_stencil.stencilTestEnable = VK_FALSE;			// unused. typically used for reflection, shadow...
	depth_stencil.front = {};
	depth_stencil.back = {};
	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;

	/* DYNAMIC STATE =========================================================== */
	std::array<VkDynamicState, 2>	dynamic_states = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo	dynamic_state{};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
	dynamic_state.pDynamicStates = dynamic_states.data();

	/* PIPELINE ================================================================ */
	VkGraphicsPipelineCreateInfo	pipeline_info{};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.pVertexInputState = &vert_input;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizing;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pDepthStencilState = &depth_stencil;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = &dynamic_state;
	pipeline_info.layout = _pipeline_layout;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
	pipeline_info.basePipelineIndex = -1;

	SCOP_DEBUG("PipelineManager::_assembleGraphicsPipelines: scene");
	_graphics.scene->assemble(device, pipeline_info);
	SCOP_DEBUG("Scene pipeline assembled.");

	color_blending.attachmentCount = 0;
	rasterizing.cullMode = VK_CULL_MODE_NONE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;

	SCOP_DEBUG("PipelineManager::_assembleGraphicsPipelines: shadows");
	_graphics.shadows->assemble(device, pipeline_info);
	SCOP_DEBUG("Shadowmap pipeline assembled.");
}

} // namespace scop::gfx