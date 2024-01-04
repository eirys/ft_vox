/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_descriptor_set.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 11:08:11 by etran             #+#    #+#             */
/*   Updated: 2024/01/04 01:19:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_descriptor_set.h"
#include "device.h"
#include "buffer.h"
#include "uniform_buffer_object.h"
#include "texture_handler.h"
#include "input_handler.h"
#include "utils.h"

#include <array> // std::array
#include <stdexcept> // std::runtime_error

namespace scop::gfx {

enum class ScenePipelineDescriptors: uint32_t {
	Camera			= 0,
	Projector		= 1,
	Light			= 2,
	BlockTextures	= 3,
	ShadowMap		= 4,
	ChunkMap		= 5,
	VerticesData	= 6,

	First = Camera,
	Last = VerticesData
};

constexpr const uint32_t DescriptorSetSize = scop::utils::enumSize<ScenePipelineDescriptors>();

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Setup descriptor layout
*/
void	SceneDescriptorSet::init(scop::core::Device& device) {
	std::array<VkDescriptorSetLayoutBinding, DescriptorSetSize>	bindings = {
		super::createLayoutBinding(DescriptorType::UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, (uint32_t)ScenePipelineDescriptors::Camera),
		super::createLayoutBinding(DescriptorType::UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, (uint32_t)ScenePipelineDescriptors::Projector),
		super::createLayoutBinding(DescriptorType::UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, (uint32_t)ScenePipelineDescriptors::Light),
		super::createLayoutBinding(DescriptorType::COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, (uint32_t)ScenePipelineDescriptors::BlockTextures),
		super::createLayoutBinding(DescriptorType::COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, (uint32_t)ScenePipelineDescriptors::ShadowMap),
		super::createLayoutBinding(DescriptorType::COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_VERTEX_BIT, (uint32_t)ScenePipelineDescriptors::ChunkMap),
		super::createLayoutBinding(DescriptorType::STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, (uint32_t)ScenePipelineDescriptors::VerticesData)
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
void	SceneDescriptorSet::fill(
	scop::core::Device& device,
	const Buffer& buffer,
	TextureHandlerPtr textures,
	TextureHandlerPtr shadowmap,
	const scop::InputHandler& input
) {
	VkDescriptorBufferInfo	camera_info{};
	camera_info.buffer = buffer.getBuffer();
	camera_info.offset = offsetof(UniformBufferObject, camera);
	camera_info.range = sizeof(UniformBufferObject::Camera);

	VkDescriptorBufferInfo	projector_info{};
	projector_info.buffer = buffer.getBuffer();
	projector_info.offset = offsetof(UniformBufferObject, projector);
	projector_info.range = sizeof(UniformBufferObject::Camera);

	VkDescriptorBufferInfo	light_info{};
	light_info.buffer = buffer.getBuffer();
	light_info.offset = offsetof(UniformBufferObject, light);
	light_info.range = sizeof(UniformBufferObject::Light);

	VkDescriptorImageInfo	texture_info{};
	texture_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	texture_info.imageView = textures->getTextureBuffer().getView();
	texture_info.sampler = textures->getTextureSampler();

	VkDescriptorImageInfo	depth_info{};
	depth_info.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
	depth_info.imageView = shadowmap->getTextureBuffer().getView();
	depth_info.sampler = shadowmap->getTextureSampler();

	VkDescriptorImageInfo	chunk_info{};
	chunk_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	chunk_info.imageView = input.getChunkMap()->getTextureBuffer().getView();
	chunk_info.sampler = input.getChunkMap()->getTextureSampler();

	VkDescriptorBufferInfo	vertices_data_info{};
	vertices_data_info.buffer = input.getInputBuffer().getBuffer();
	vertices_data_info.offset = (uint32_t)InputBufferOffset::VerticesData;
	vertices_data_info.range = (uint32_t)InputBufferSize::VerticesData;

	std::array<VkWriteDescriptorSet, DescriptorSetSize>	writes{
		super::createWriteDescriptorSet(DescriptorType::UNIFORM_BUFFER, &camera_info, (uint32_t)ScenePipelineDescriptors::Camera),
		super::createWriteDescriptorSet(DescriptorType::UNIFORM_BUFFER, &projector_info, (uint32_t)ScenePipelineDescriptors::Projector),
		super::createWriteDescriptorSet(DescriptorType::UNIFORM_BUFFER, &light_info, (uint32_t)ScenePipelineDescriptors::Light),
		super::createWriteDescriptorSet(DescriptorType::COMBINED_IMAGE_SAMPLER, &texture_info, (uint32_t)ScenePipelineDescriptors::BlockTextures),
		super::createWriteDescriptorSet(DescriptorType::COMBINED_IMAGE_SAMPLER, &depth_info, (uint32_t)ScenePipelineDescriptors::ShadowMap),
		super::createWriteDescriptorSet(DescriptorType::COMBINED_IMAGE_SAMPLER, &chunk_info, (uint32_t)ScenePipelineDescriptors::ChunkMap),
		super::createWriteDescriptorSet(DescriptorType::STORAGE_BUFFER, &vertices_data_info, (uint32_t)ScenePipelineDescriptors::VerticesData)
	};

	vkUpdateDescriptorSets(
		device.getLogicalDevice(),
		DescriptorSetSize, writes.data(),
		0, nullptr);
}

} // namespace scop::gfx