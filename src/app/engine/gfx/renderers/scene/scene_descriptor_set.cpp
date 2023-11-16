/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_descriptor_set.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 11:08:11 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:01:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_descriptor_set.h"
#include "device.h"
#include "buffer.h"
#include "uniform_buffer_object.h"
#include "texture_handler.h"
#include "input_handler.h"

#include <array> // std::array
#include <stdexcept> // std::runtime_error

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Setup descriptor layout
*/
void	SceneDescriptorSet::init(scop::core::Device& device) {
	VkDescriptorSetLayoutBinding	camera{};
	camera.binding = 0;
	camera.descriptorCount = 1;
	camera.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	camera.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	++super::_writes_sizes.uniform_buffer;

	VkDescriptorSetLayoutBinding	projector{};
	projector.binding = 1;
	projector.descriptorCount = 1;
	projector.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	projector.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	++super::_writes_sizes.uniform_buffer;

	VkDescriptorSetLayoutBinding	light{};
	light.binding = 2;
	light.descriptorCount = 1;
	light.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	light.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	++super::_writes_sizes.uniform_buffer;

	VkDescriptorSetLayoutBinding	texture{};
	texture.binding = 3;
	texture.descriptorCount = 1;
	texture.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	texture.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	++super::_writes_sizes.combined_image_sampler;

	VkDescriptorSetLayoutBinding	depth{};
	depth.binding = 4;
	depth.descriptorCount = 1;
	depth.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	depth.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	++super::_writes_sizes.combined_image_sampler;

	VkDescriptorSetLayoutBinding	height{};
	height.binding = 5;
	height.descriptorCount = 1;
	height.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	height.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	++super::_writes_sizes.combined_image_sampler;

	VkDescriptorSetLayoutBinding	culling{};
	culling.binding = 6;
	culling.descriptorCount = 1;
	culling.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	culling.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	++super::_writes_sizes.combined_image_sampler;

	std::array<VkDescriptorSetLayoutBinding, 7>	bindings = {
		camera,
		projector,
		light,
		texture,
		depth,
		height,
		culling };

	VkDescriptorSetLayoutCreateInfo	layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
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
void	SceneDescriptorSet::plug(
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

	VkDescriptorImageInfo	height_info{};
	height_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	height_info.imageView = input.getHeightMap()->getTextureBuffer().getView();
	height_info.sampler = input.getHeightMap()->getTextureSampler();

	VkDescriptorImageInfo	cull_info{};
	cull_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	cull_info.imageView = input.getChunkMap()->getTextureBuffer().getView();
	cull_info.sampler = input.getChunkMap()->getTextureSampler();

	std::array<VkWriteDescriptorSet, 7>	writes{};

	// Camera UBO
	writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[0].dstSet = super::_set;
	writes[0].dstBinding = 0;
	writes[0].dstArrayElement = 0;
	writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[0].descriptorCount = 1;
	writes[0].pBufferInfo = &camera_info;
	writes[0].pImageInfo = nullptr;
	writes[0].pTexelBufferView = nullptr;

	// Projector UBO
	writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[1].dstSet = super::_set;
	writes[1].dstBinding = 1;
	writes[1].dstArrayElement = 0;
	writes[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[1].descriptorCount = 1;
	writes[1].pBufferInfo = &projector_info;
	writes[1].pImageInfo = nullptr;
	writes[1].pTexelBufferView = nullptr;

	// Light UBO
	writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[2].dstSet = super::_set;
	writes[2].dstBinding = 2;
	writes[2].dstArrayElement = 0;
	writes[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[2].descriptorCount = 1;
	writes[2].pBufferInfo = &light_info;
	writes[2].pImageInfo = nullptr;
	writes[2].pTexelBufferView = nullptr;

	// Texture sampler
	writes[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[3].dstSet = super::_set;
	writes[3].dstBinding = 3;
	writes[3].dstArrayElement = 0;
	writes[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[3].descriptorCount = 1;
	writes[3].pBufferInfo = nullptr;
	writes[3].pImageInfo = &texture_info;
	writes[3].pTexelBufferView = nullptr;

	// Depth sampler
	writes[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[4].dstSet = super::_set;
	writes[4].dstBinding = 4;
	writes[4].dstArrayElement = 0;
	writes[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[4].descriptorCount = 1;
	writes[4].pBufferInfo = nullptr;
	writes[4].pImageInfo = &depth_info;
	writes[4].pTexelBufferView = nullptr;

	// Height sampler
	writes[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[5].dstSet = super::_set;
	writes[5].dstBinding = 5;
	writes[5].dstArrayElement = 0;
	writes[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[5].descriptorCount = 1;
	writes[5].pBufferInfo = nullptr;
	writes[5].pImageInfo = &height_info;
	writes[5].pTexelBufferView = nullptr;

	// Height sampler
	writes[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[6].dstSet = super::_set;
	writes[6].dstBinding = 6;
	writes[6].dstArrayElement = 0;
	writes[6].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[6].descriptorCount = 1;
	writes[6].pBufferInfo = nullptr;
	writes[6].pImageInfo =  &cull_info;
	writes[6].pTexelBufferView = nullptr;

	vkUpdateDescriptorSets(
		device.getLogicalDevice(),
		static_cast<uint32_t>(writes.size()), writes.data(),
		0, nullptr);
}

} // namespace scop::gfx