/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_descriptor_set.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 11:34:47 by etran             #+#    #+#             */
/*   Updated: 2024/01/03 14:44:39 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shadows_descriptor_set.h"
#include "device.h"
#include "buffer.h"
#include "uniform_buffer_object.h"
#include "texture_handler.h"
#include "input_handler.h"
#include "utils.h"

#include <stdexcept> // std:runtime_error
#include <array> // std::array

namespace scop::gfx {

enum class ShadowsPipelineDescriptors: uint32_t {
	Projector,
	ChunkMap,
	VerticesData,

	First = Projector,
	Last = VerticesData
};

constexpr const uint32_t DescriptorSetSize = scop::utils::enumSize<ShadowsPipelineDescriptors>();

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Setup descriptor layout
*/
void	ShadowsDescriptorSet::init(scop::core::Device& device) {
	std::array<VkDescriptorSetLayoutBinding, DescriptorSetSize>	bindings = {
		super::createLayoutBinding(DescriptorType::UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, (uint32_t)ShadowsPipelineDescriptors::Projector),
		super::createLayoutBinding(DescriptorType::COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_VERTEX_BIT, (uint32_t)ShadowsPipelineDescriptors::ChunkMap),
		super::createLayoutBinding(DescriptorType::STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, (uint32_t)ShadowsPipelineDescriptors::VerticesData)
	};

	VkDescriptorSetLayoutCreateInfo	layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = DescriptorSetSize;
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device.getLogicalDevice(), &layout_info, nullptr, &(super::_layout)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}
}

/**
 * @brief Link descriptor with pipeline resources.
 *
 * @note Should be called after the set has been allocated.
*/
void	ShadowsDescriptorSet::fill(
	scop::core::Device& device,
	const Buffer& buffer,
	const InputHandler& input
) {
	VkDescriptorBufferInfo	projector_info{};
	projector_info.buffer = buffer.getBuffer();
	projector_info.offset = offsetof(UniformBufferObject, projector);
	projector_info.range = sizeof(UniformBufferObject::Camera);

	VkDescriptorImageInfo	chunk_info{};
	chunk_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	chunk_info.imageView = input.getChunkMap()->getTextureBuffer().getView();
	chunk_info.sampler = input.getChunkMap()->getTextureSampler();

	VkDescriptorBufferInfo	vertices_data_info{};
	vertices_data_info.buffer = input.getInputBuffer().getBuffer();
	vertices_data_info.offset = (uint32_t)InputBufferOffset::VerticesData;
	vertices_data_info.range = (uint32_t)InputBufferSize::VerticesData;

	std::array<VkWriteDescriptorSet, DescriptorSetSize> writes{
		super::createWriteDescriptorSet(DescriptorType::UNIFORM_BUFFER, &projector_info, (uint32_t)ShadowsPipelineDescriptors::Projector),
		super::createWriteDescriptorSet(DescriptorType::COMBINED_IMAGE_SAMPLER, &chunk_info, (uint32_t)ShadowsPipelineDescriptors::ChunkMap),
		super::createWriteDescriptorSet(DescriptorType::STORAGE_BUFFER, &vertices_data_info, (uint32_t)ShadowsPipelineDescriptors::VerticesData)
	};

	vkUpdateDescriptorSets(
		device.getLogicalDevice(),
		DescriptorSetSize, writes.data(),
		0, nullptr
	);
}

} // namespace scop::gfx