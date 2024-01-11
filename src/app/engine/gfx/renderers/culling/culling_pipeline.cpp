/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_pipeline.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 11:50:07 by etran             #+#    #+#             */
/*   Updated: 2024/01/08 14:55:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "culling_pipeline.h"
#include "device.h"
#include "command_buffer.h"
#include "pipeline_helpers.h"

#include "culling_descriptor_set.h"
#include "culling_texture_handler.h"

#include <stdexcept> // std::runtime_error

#if defined(__LINUX)
# define CULLING_COMPUTE_PATH	"/culling.compute.spv"
#else
# define CULLING_COMPUTE_PATH	"/culling_comp.spv"
#endif

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

CullingPipeline::CullingPipeline() {
	super::_descriptor = std::make_shared<CullingDescriptorSet>();
}

/* ========================================================================== */

void CullingPipeline::init(scop::core::Device& device) {
	super::_descriptor->init(device);
}

void	CullingPipeline::assemble(
	scop::core::Device& device,
	VkComputePipelineCreateInfo& info
) {
	VkShaderModule	compute_module = createShaderModule(device, CULLING_COMPUTE_PATH);

	VkPipelineShaderStageCreateInfo	compute_info{};
	compute_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	compute_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	compute_info.module = compute_module;
	compute_info.pName = "main";

	info.stage = compute_info;

	if (vkCreateComputePipelines(device.getLogicalDevice(), VK_NULL_HANDLE, 1, &info, nullptr, &(super::_pipeline)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create culling compute pipeline");
	}

	vkDestroyShaderModule(device.getLogicalDevice(), compute_module, nullptr);
}

void	CullingPipeline::plugDescriptor(
	scop::core::Device& device,
	const scop::InputHandler& input
) {
	auto culling_descriptor = std::dynamic_pointer_cast<CullingDescriptorSet>(super::_descriptor);
	culling_descriptor->fill(device, input);
}

void	CullingPipeline::compute(
	scop::core::Device& device,
	VkPipelineLayout layout,
	CommandBuffer& command_buffer
) {
	// Make sure previous compute shader is finished
	vkQueueWaitIdle(device.getComputeQueue());

	command_buffer.reset();
	command_buffer.begin(0);

	std::array<VkDescriptorSet, 1>	descriptor_sets = { super::_descriptor->getSet() };

	vkCmdBindDescriptorSets(
		command_buffer.getBuffer(),
		VK_PIPELINE_BIND_POINT_COMPUTE,
		layout,
		super::_descriptor->getSetIndex(),
		static_cast<uint32_t>(descriptor_sets.size()), descriptor_sets.data(),
		0, nullptr);
	vkCmdBindPipeline(
		command_buffer.getBuffer(),
		VK_PIPELINE_BIND_POINT_COMPUTE,
		super::_pipeline);

	// TODO Need to optimize dispatch sizes with vkGetPhysicalDeviceProperties
	vkCmdDispatch(command_buffer.getBuffer(), 8, 8, 8);

	command_buffer.end(device);
}

/* ========================================================================== */

CullingPipeline::TextureHandlerPtr	CullingPipeline::getCullingTextureHandler() const {
	return _culling_texture;
}

} // namespace scop::gfx