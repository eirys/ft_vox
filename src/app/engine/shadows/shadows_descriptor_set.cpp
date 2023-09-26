/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_descriptor_set.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 11:34:47 by etran             #+#    #+#             */
/*   Updated: 2023/09/01 17:22:34 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shadows_descriptor_set.h"
#include "device.h"
#include "buffer.h"
#include "uniform_buffer_object.h"
#include "texture_handler.h"

#include <stdexcept> // std:runtime_error
#include <array> // std::array

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Setup descriptor layout
*/
void	ShadowsDescriptorSet::init(Device& device) {
	VkDescriptorSetLayoutBinding	projector{};
	projector.binding = 0;
	projector.descriptorCount = 1;
	projector.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	projector.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding	height{};
	height.binding = 1;
	height.descriptorCount = 1;
	height.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	height.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2>	bindings = {
		projector,
		height };

	VkDescriptorSetLayoutCreateInfo	layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device.getLogicalDevice(), &layout_info, nullptr, &(super::_layout)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}

	super::_writes_sizes.uniform_buffer = 1;
	super::_writes_sizes.combined_image_sampler = 1;
}

/**
 * @brief Link descriptor with pipeline resources.
 *
 * @note Should be called after the set has been allocated.
*/
void	ShadowsDescriptorSet::plug(
	Device& device,
	Buffer& buffer,
	TextureHandlerPtr heightmap
) {
	std::array<VkWriteDescriptorSet, 2> writes{};

	VkDescriptorBufferInfo	projector_info{};
	projector_info.buffer = buffer.getBuffer();
	projector_info.offset = offsetof(UniformBufferObject, projector);
	projector_info.range = sizeof(UniformBufferObject::Camera);

	VkDescriptorImageInfo	height_info{};
	height_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	height_info.imageView = heightmap->getTextureBuffer().getView();
	height_info.sampler = heightmap->getTextureSampler();

	writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[0].dstSet = super::_set;
	writes[0].dstBinding = 0;
	writes[0].dstArrayElement = 0;
	writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[0].descriptorCount = 1;
	writes[0].pBufferInfo = &projector_info;
	writes[0].pImageInfo = nullptr;
	writes[0].pTexelBufferView = nullptr;

	// Height sampler
	writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[1].dstSet = super::_set;
	writes[1].dstBinding = 1;
	writes[1].dstArrayElement = 0;
	writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[1].descriptorCount = 1;
	writes[1].pBufferInfo = nullptr;
	writes[1].pImageInfo = &height_info;
	writes[1].pTexelBufferView = nullptr;

	vkUpdateDescriptorSets(
		device.getLogicalDevice(),
		static_cast<uint32_t>(writes.size()), writes.data(),
		0, nullptr
	);
}

} // namespace scop::graphics