/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_descriptor_set.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 16:54:52 by etran             #+#    #+#             */
/*   Updated: 2023/12/26 19:39:28 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "culling_descriptor_set.h"
#include "device.h"

// GFX
#include "buffer.h"
#include "input_handler.h"
#include "texture_handler.h"

#include <stdexcept> // std::runtime_error

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void CullingDescriptorSet::init(scop::core::Device& device) {
	VkDescriptorSetLayoutBinding	frustum{};
	frustum.binding = 0;
	frustum.descriptorCount = 1;
	frustum.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	frustum.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	++super::_writes_sizes.uniform_buffer;

	VkDescriptorSetLayoutBinding	chunk{};
	chunk.binding = 1;
	chunk.descriptorCount = 1;
	chunk.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	chunk.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	++super::_writes_sizes.combined_image_sampler;

	VkDescriptorSetLayoutBinding	quad_count{};
	quad_count.binding = 2;
	quad_count.descriptorCount = 1;
	quad_count.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	quad_count.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	++super::_writes_sizes.storage_buffer;

	VkDescriptorSetLayoutBinding	vertices_data{};
	vertices_data.binding = 3;
	vertices_data.descriptorCount = 1;
	vertices_data.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	vertices_data.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	++super::_writes_sizes.storage_buffer;

	std::array<VkDescriptorSetLayoutBinding, 4>	bindings = {
		frustum,
		chunk,
		quad_count,
		vertices_data };

	VkDescriptorSetLayoutCreateInfo	layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device.getLogicalDevice(), &layout_info, nullptr, &(super::_layout)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}
}

void	CullingDescriptorSet::plug(
	scop::core::Device& device,
	const scop::InputHandler& input
) {
	VkDescriptorBufferInfo	frustum_info{};
	frustum_info.buffer = input.getInputBuffer().getBuffer();
	frustum_info.offset = (uint32_t)InputBufferOffset::Frustum;
	frustum_info.range = (uint32_t)InputBufferSize::Frustum;

	VkDescriptorImageInfo	chunk_info{};
	chunk_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	chunk_info.imageView = input.getChunkMap()->getTextureBuffer().getView();
	chunk_info.sampler = input.getChunkMap()->getTextureSampler();

	VkDescriptorBufferInfo	quad_count{};
	frustum_info.buffer = input.getInputBuffer().getBuffer();
	frustum_info.offset = (uint32_t)InputBufferOffset::QuadCount;
	frustum_info.range = (uint32_t)InputBufferSize::QuadCount;

	VkDescriptorBufferInfo	vertices_data{};
	vertices_data.buffer = input.getInputBuffer().getBuffer();
	vertices_data.offset = (uint32_t)InputBufferOffset::VerticesData;
	vertices_data.range = (uint32_t)InputBufferSize::VerticesData;

	std::array<VkWriteDescriptorSet, 4>	writes = {
		super::createWriteDescriptorSet(DescriptorType::UNIFORM_BUFFER, &frustum_info, 0),
		super::createWriteDescriptorSet(DescriptorType::COMBINED_IMAGE_SAMPLER, &chunk_info, 1),
		super::createWriteDescriptorSet(DescriptorType::STORAGE_BUFFER, &quad_count, 2),
		super::createWriteDescriptorSet(DescriptorType::STORAGE_BUFFER, &vertices_data, 3) };

	vkUpdateDescriptorSets(
		device.getLogicalDevice(),
		static_cast<uint32_t>(writes.size()), writes.data(),
		0, nullptr);
}


} // namespace scop::gfx