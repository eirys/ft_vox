/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:33:52 by etran             #+#    #+#             */
/*   Updated: 2023/08/15 20:58:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_set.h"
#include "device.h"
#include "texture_handler.h"
#include "uniform_buffer_object.h"
#include "descriptor_pool.h"

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	DescriptorSet::init(
	Device& device,
	TextureHandler& texture_handler,
	const ::scop::UniformBufferObject& ubo
) {
	_createLayout(device);
	_createUniformBuffers(device);
	_createWrites(texture_handler);
	_initUniformBuffer(ubo);
}

void	DescriptorSet::destroy(Device& device) {
	_buffer.unmap(device.getLogicalDevice());
	_buffer.destroy(device.getLogicalDevice());

	vkDestroyDescriptorSetLayout(
		device.getLogicalDevice(),
		_layout,
		nullptr
	);
}

/**
 * @brief	Copies updated ubo to the uniform buffer.
*/
void	DescriptorSet::update(
	const ::scop::UniformBufferObject& ubo
) noexcept {
	_buffer.copyFrom(&ubo, sizeof(UniformBufferObject));
}

/**
 * @brief	Destroys writes data after they have been used.
*/
void	DescriptorSet::removeWrites(Device& device) {
	_writes.buffer_infos.clear();
	_writes.image_infos.clear();
	_writes.writes_data.clear();
}


/* ========================================================================== */

VkDescriptorSetLayout	DescriptorSet::getLayout() const noexcept {
	return _layout;
}

VkDescriptorSet	DescriptorSet::getSet() const noexcept {
	return _set;
}

DescriptorSet::DescriptorSizes	DescriptorSet::getPoolSizes() const noexcept {
	return {
		.uniform_buffer = 3,
		.combined_image_sampler = 2
	};
}

const std::vector<VkWriteDescriptorSet>&	DescriptorSet::getWrites() const noexcept {
	return _writes.writes_data;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	DescriptorSet::_createLayout(Device& device) {
	// Camera UBO layout
	VkDescriptorSetLayoutBinding	camera_binding{};
	camera_binding.binding = 0;
	camera_binding.descriptorCount = 1;
	camera_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	camera_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	// Sampler layout
	VkDescriptorSetLayoutBinding	sampler_binding{};
	sampler_binding.binding = 1;
	sampler_binding.descriptorCount = 1;
	sampler_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	sampler_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	// Light UBO layout
	VkDescriptorSetLayoutBinding	light_binding{};
	light_binding.binding = 2;
	light_binding.descriptorCount = 1;
	light_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	light_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	// Projector UBO layout
	VkDescriptorSetLayoutBinding	projector_binding{};
	projector_binding.binding = 3;
	projector_binding.descriptorCount = 1;
	projector_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	projector_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


	// TODO 5
	std::array<VkDescriptorSetLayoutBinding, 5>	bindings = {
		camera_binding,
		sampler_binding,
		light_binding,
		projector_binding
	};

	VkDescriptorSetLayoutCreateInfo	layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device.getLogicalDevice(), &layout_info, nullptr, &_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}
}

void	DescriptorSet::_createWrites(
	TextureHandler& texture_handler
) {
	_writes.buffer_infos.reserve(3);
	_writes.image_infos.reserve(1);
	// _writes.buffer_infos.reserve(3);
	// _writes.image_infos.reserve(2);

	// Camera
	_writes.buffer_infos[0].buffer = _buffer.getBuffer();
	_writes.buffer_infos[0].offset = offsetof(UniformBufferObject, camera);
	_writes.buffer_infos[0].range = sizeof(UniformBufferObject::Camera);

	// Light
	_writes.buffer_infos[1].buffer = _buffer.getBuffer();
	_writes.buffer_infos[1].offset = offsetof(UniformBufferObject, light);
	_writes.buffer_infos[1].range = sizeof(UniformBufferObject::Light);

	// Projector
	_writes.buffer_infos[2].buffer = _buffer.getBuffer();
	_writes.buffer_infos[2].offset = offsetof(UniformBufferObject, projector);
	_writes.buffer_infos[2].range = sizeof(UniformBufferObject::Camera);

	// Texture
	_writes.image_infos[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	_writes.image_infos[0].imageView = texture_handler.getTextureBuffer().getView();
	_writes.image_infos[0].sampler = texture_handler.getTextureSampler();

	// Shadowmap
	// VkDescriptorImageInfo	shadowmap_info{};
	// shadowmap_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	// shadowmap_info.imageView = shadowmap.getTextureBuffer().getView();
	// shadowmap_info.sampler = shadowmap.getTextureSampler();

	_writes.writes_data.reserve(4);
	// _writes.writes_data.reserve(5);

	// Camera
	_writes.writes_data[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes.writes_data[0].dstSet = _set;
	_writes.writes_data[0].dstBinding = 0;
	_writes.writes_data[0].dstArrayElement = 0;
	_writes.writes_data[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	_writes.writes_data[0].descriptorCount = 1;
	_writes.writes_data[0].pBufferInfo = &_writes.buffer_infos[0];
	_writes.writes_data[0].pImageInfo = nullptr;
	_writes.writes_data[0].pTexelBufferView = nullptr;

	// Texture
	_writes.writes_data[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes.writes_data[1].dstSet = _set;
	_writes.writes_data[1].dstBinding = 1;
	_writes.writes_data[1].dstArrayElement = 0;
	_writes.writes_data[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	_writes.writes_data[1].descriptorCount = 1;
	_writes.writes_data[1].pBufferInfo = nullptr;
	_writes.writes_data[1].pImageInfo = &_writes.image_infos[0];
	_writes.writes_data[1].pTexelBufferView = nullptr;

	// Light
	_writes.writes_data[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes.writes_data[2].dstSet = _set;
	_writes.writes_data[2].dstBinding = 2;
	_writes.writes_data[2].dstArrayElement = 0;
	_writes.writes_data[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	_writes.writes_data[2].descriptorCount = 1;
	_writes.writes_data[2].pBufferInfo = &_writes.buffer_infos[1];
	_writes.writes_data[2].pImageInfo = nullptr;
	_writes.writes_data[2].pTexelBufferView = nullptr;

	// Projector
	_writes.writes_data[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes.writes_data[3].dstSet = _set;
	_writes.writes_data[3].dstBinding = 3;
	_writes.writes_data[3].dstArrayElement = 0;
	_writes.writes_data[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	_writes.writes_data[3].descriptorCount = 1;
	_writes.writes_data[3].pBufferInfo = &_writes.buffer_infos[2];
	_writes.writes_data[3].pImageInfo = nullptr;
	_writes.writes_data[3].pTexelBufferView = nullptr;

	// Shadowmap
	// _writes[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	// _writes[4].dstSet = _set;
	// _writes[4].dstBinding = 1;
	// _writes[4].dstArrayElement = 0;
	// _writes[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// _writes[4].descriptorCount = 1;
	// _writes[4].pBufferInfo = nullptr;
	// _writes[4].pImageInfo = std::make_shared<VkDescriptorImageInfo>(shadowmap_info).get();
	// _writes[4].pTexelBufferView = nullptr;

	// vkUpdateDescriptorSets(
	// 	device.getLogicalDevice(),
	// 	static_cast<uint32_t>(_writes.size()),
	// 	_writes.data(),
	// 	0, nullptr
	// );
}

void	DescriptorSet::_createUniformBuffers(Device& device) {
	// Camera and texture are dynamically updated.
	VkDeviceSize	buffer_size = sizeof(UniformBufferObject);

	// Create the buffer and allocate memory
	_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Map it to allow CPU to write on it
	_buffer.map(device.getLogicalDevice(), buffer_size);
}

/**
 * @brief	Initiate uniform buffer.
*/
void	DescriptorSet::_initUniformBuffer(
	const ::scop::UniformBufferObject& ubo
) noexcept {
	// UniformBufferObject	ubo{};
	// ubo.light = light;

	// ubo.projector.proj =
	// 	scop::orthographic(1, 20, 1, 20, 1, 1000);
	// 	//scop::perspective(45, 1, 1, 1000);
	// ubo.projector.view = scop::lookAt(
	// 	scop::Vect3(80, 20, 80),
	// 	scop::Vect3(80, 0, 80),
	// 	scop::normalize(scop::Vect3(1, 0.02, 0.0)));
	// ubo.projector.proj[5] *= -1;

	_buffer.copyFrom(&ubo, sizeof(UniformBufferObject));
}

// /**
//  * Update the camera part of the uniform buffer.
// */
// void	DescriptorSet::_updateCamera(
// 	VkExtent2D extent,
// 	const vox::Player& player
// ) {
// 	UniformBufferObject::Camera	camera{};

// 	// Define camera view
// 	camera.view = scop::lookAtDir(
// 		player.getPosition(),
// 		player.getEyeDir(),
// 		scop::Vect3(0.0f, 1.0f, 0.0f)
// 	);

// 	// Define persp. projection transformation
// 	camera.proj = scop::perspective(
// 		scop::math::radians(70.0f),
// 		extent.width / static_cast<float>(extent.height),
// 		0.1f,
// 		1000.0f
// 	);
// 	// Invert y axis (because y axis is inverted in Vulkan)
// 	camera.proj[5] *= -1;

// 	// Copy to uniform buffer
// 	_buffer.copyFrom(
// 		&camera,
// 		sizeof(UniformBufferObject::Camera),
// 		offsetof(UniformBufferObject, camera)
// 	);
// }


} // namespace scop::graphics