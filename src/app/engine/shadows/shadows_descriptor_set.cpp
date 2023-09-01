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

#include <stdexcept> // std:runtime_error

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

	VkDescriptorSetLayoutCreateInfo	layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = 1;
	layout_info.pBindings = &projector;

	if (vkCreateDescriptorSetLayout(device.getLogicalDevice(), &layout_info, nullptr, &(super::_layout)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}

	super::_writes_sizes.uniform_buffer = 1;
	super::_writes_sizes.combined_image_sampler = 0;
}

/**
 * @brief Link descriptor with pipeline resources.
 *
 * @note Should be called after the set has been allocated.
*/
void	ShadowsDescriptorSet::plug(
	Device& device,
	Buffer& buffer
) {
	VkWriteDescriptorSet write{};

	VkDescriptorBufferInfo	projector_info{};
	projector_info.buffer = buffer.getBuffer();
	projector_info.offset = offsetof(UniformBufferObject, projector);
	projector_info.range = sizeof(UniformBufferObject::Camera);

	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = super::_set;
	write.dstBinding = 0;
	write.dstArrayElement = 0;
	write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	write.descriptorCount = 1;
	write.pBufferInfo = &projector_info;
	write.pImageInfo = nullptr;
	write.pTexelBufferView = nullptr;

	vkUpdateDescriptorSets(
		device.getLogicalDevice(),
		1, &write,
		0, nullptr
	);
}

} // namespace scop::graphics