/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_descriptor_set.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 16:54:52 by etran             #+#    #+#             */
/*   Updated: 2024/01/11 14:49:40 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "culling_descriptor_set.h"
#include "device.h"

// GFX
#include "buffer.h"
#include "input_handler.h"
#include "texture_handler.h"

#include "utils.h"

#include <stdexcept> // std::runtime_error
#include <cassert> // assert

namespace scop::gfx {

enum class CullingPipelineDescriptors: uint32_t {
	Frustum = 0,
	ChunkMap,
	QuadCount,
	VerticesData,

	First = Frustum,
	Last = VerticesData
};

constexpr const uint32_t DescriptorSetSize = scop::utils::enumSize<CullingPipelineDescriptors>();

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

CullingDescriptorSet::CullingDescriptorSet(): DescriptorSet(DescriptorIndex::Culling) {}

/* ========================================================================== */

void CullingDescriptorSet::init(scop::core::Device& device) {
	std::array<VkDescriptorSetLayoutBinding, DescriptorSetSize>	bindings = {
		super::createLayoutBinding(DescriptorType::UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, (uint32_t)CullingPipelineDescriptors::Frustum),
		super::createLayoutBinding(DescriptorType::COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_COMPUTE_BIT, (uint32_t)CullingPipelineDescriptors::ChunkMap),
		super::createLayoutBinding(DescriptorType::STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, (uint32_t)CullingPipelineDescriptors::QuadCount),
		super::createLayoutBinding(DescriptorType::STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, (uint32_t)CullingPipelineDescriptors::VerticesData)
	};

	VkDescriptorSetLayoutCreateInfo	layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = DescriptorSetSize;
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device.getLogicalDevice(), &layout_info, nullptr, &(super::_layout)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}
}

void	CullingDescriptorSet::fill(
	scop::core::Device& device,
	const scop::InputHandler& input
) {
	VkDescriptorBufferInfo	frustum_info{};
	frustum_info.buffer = input.getFrustumBuffer().getBuffer();
	frustum_info.offset = (uint32_t)InputBufferOffset::Frustum;
	frustum_info.range = (uint32_t)InputBufferSize::Frustum;
	assert(frustum_info.buffer != VK_NULL_HANDLE);

	VkDescriptorImageInfo	chunk_info{};
	chunk_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	chunk_info.imageView = input.getChunkMap()->getTextureBuffer().getView();
	chunk_info.sampler = input.getChunkMap()->getTextureSampler();

	VkDescriptorBufferInfo	quad_count_info{};
	quad_count_info.buffer = input.getQuadCountBuffer().getBuffer();
	quad_count_info.offset = (uint32_t)InputBufferOffset::QuadCount;
	quad_count_info.range = (uint32_t)InputBufferSize::QuadCount;
	assert(quad_count_info.buffer != VK_NULL_HANDLE);

	VkDescriptorBufferInfo	vertices_data_info{};
	vertices_data_info.buffer = input.getVerticesDataBuffer().getBuffer();
	vertices_data_info.offset = (uint32_t)InputBufferOffset::VerticesData;
	vertices_data_info.range = (uint32_t)InputBufferSize::VerticesData;
	assert(vertices_data_info.buffer != VK_NULL_HANDLE);

	std::array<VkWriteDescriptorSet, DescriptorSetSize>	writes = {
		super::createWriteDescriptorSet(DescriptorType::UNIFORM_BUFFER, &frustum_info, (uint32_t)CullingPipelineDescriptors::Frustum),
		super::createWriteDescriptorSet(DescriptorType::COMBINED_IMAGE_SAMPLER, &chunk_info, (uint32_t)CullingPipelineDescriptors::ChunkMap),
		super::createWriteDescriptorSet(DescriptorType::STORAGE_BUFFER, &quad_count_info, (uint32_t)CullingPipelineDescriptors::QuadCount),
		super::createWriteDescriptorSet(DescriptorType::STORAGE_BUFFER, &vertices_data_info, (uint32_t)CullingPipelineDescriptors::VerticesData)
	};

	vkUpdateDescriptorSets(
		device.getLogicalDevice(),
		DescriptorSetSize, writes.data(),
		0, nullptr);
}


} // namespace scop::gfx