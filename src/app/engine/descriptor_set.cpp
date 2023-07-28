/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:56:05 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 10:11:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_set.h"
#include "engine.h"

#include <vector>
#include <array>
#include <stdexcept> // std::runtime_error
#include <chrono> // std::chrono
#include <optional> // std::optional

namespace scop {
namespace graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Descriptor set layout for uniform buffer and combined image sampler.
*/
void	DescriptorSet::initLayout(Device& device) {
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

	std::vector<VkDescriptorSetLayoutBinding>	bindings = {
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

void	DescriptorSet::initSets(
	Device& device,
	TextureSampler& texture_sampler,
	const UniformBufferObject::Light& light
) {
	uint32_t	count = static_cast<uint32_t>(
		Engine::max_frames_in_flight
	);
	_createUniformBuffers(device);
	_createDescriptorPool(device, count);
	_createDescriptorSets(
		device,
		texture_sampler,
		count
	);

	_initUniformBuffer(light);
}

void	DescriptorSet::destroy(
	Device& device
) {
	// Remove uniform buffers
	_ubo.unmap(device.getLogicalDevice());
	_ubo.destroy(device.getLogicalDevice());

	// Remove descriptor pool
	vkDestroyDescriptorPool(device.getLogicalDevice(), _pool, nullptr);
	vkDestroyDescriptorSetLayout(
		device.getLogicalDevice(),
		_layout,
		nullptr
	);
}

/**
 * Update transformation of vertices
*/
void	DescriptorSet::updateUniformBuffer(
	VkExtent2D extent,
	const vox::Player& player
) {
	_updateCamera(extent, player);
}

/* ========================================================================== */

VkDescriptorSetLayout	DescriptorSet::getLayout() const noexcept {
	return _layout;
}

VkDescriptorSet	DescriptorSet::getSet() const noexcept {
	return _set;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Handler for descriptor sets allocation.
*/
void	DescriptorSet::_createDescriptorPool(Device& device, uint32_t count) {
	std::array<VkDescriptorPoolSize, 2>	pool_sizes;

	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[0].descriptorCount = count * 3;
	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[1].descriptorCount = count;

	VkDescriptorPoolCreateInfo	pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
	pool_info.pPoolSizes = pool_sizes.data();
	pool_info.maxSets = count;

	if (vkCreateDescriptorPool(device.getLogicalDevice(), &pool_info, nullptr, &_pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool");
	}
}

void	DescriptorSet::_createDescriptorSets(
	Device& device,
	TextureSampler& texture_sampler,
	uint32_t count
) {
	VkDescriptorSetAllocateInfo	alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = _pool;
	alloc_info.descriptorSetCount = count;
	alloc_info.pSetLayouts = &_layout;

	if (vkAllocateDescriptorSets(device.getLogicalDevice(), &alloc_info, &_set) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets");
	}

	// Ubo Camera
	VkDescriptorBufferInfo	ubo_info_camera{};
	ubo_info_camera.buffer = _ubo.getBuffer();
	ubo_info_camera.offset = offsetof(UniformBufferObject, camera);
	ubo_info_camera.range = sizeof(UniformBufferObject::Camera);

	// Texture sampler
	VkDescriptorImageInfo	image_info{};
	image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	image_info.imageView = texture_sampler.getTextureBuffer().getView();
	image_info.sampler = texture_sampler.getTextureSampler();

	// Ubo light
	VkDescriptorBufferInfo	ubo_info_light{};
	ubo_info_light.buffer = _ubo.getBuffer();
	ubo_info_light.offset = offsetof(UniformBufferObject, light);
	ubo_info_light.range = sizeof(UniformBufferObject::Light);

	// Ubo projector
	VkDescriptorBufferInfo	ubo_info_projector{};
	ubo_info_projector.buffer = _ubo.getBuffer();
	ubo_info_projector.offset = offsetof(UniformBufferObject, projector);
	ubo_info_projector.range = sizeof(UniformBufferObject::Camera);

	// Allow buffer udpate using descriptor write
	std::vector<VkWriteDescriptorSet>	writes(4);

	// Camera UBO
	writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[0].dstSet = _set;
	writes[0].dstBinding = 0;
	writes[0].dstArrayElement = 0;
	writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[0].descriptorCount = 1;
	writes[0].pBufferInfo = &ubo_info_camera;
	writes[0].pImageInfo = nullptr;
	writes[0].pTexelBufferView = nullptr;

	// Sampler
	writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[1].dstSet = _set;
	writes[1].dstBinding = 1;
	writes[1].dstArrayElement = 0;
	writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writes[1].descriptorCount = 1;
	writes[1].pBufferInfo = nullptr;
	writes[1].pImageInfo = &image_info;
	writes[1].pTexelBufferView = nullptr;

	// Light UBO
	writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[2].dstSet = _set;
	writes[2].dstBinding = 2;
	writes[2].dstArrayElement = 0;
	writes[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[2].descriptorCount = 1;
	writes[2].pBufferInfo = &ubo_info_light;
	writes[2].pImageInfo = nullptr;
	writes[2].pTexelBufferView = nullptr;

	// Projector UBO
	writes[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[3].dstSet = _set;
	writes[3].dstBinding = 3;
	writes[3].dstArrayElement = 0;
	writes[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writes[3].descriptorCount = 1;
	writes[3].pBufferInfo = &ubo_info_projector;
	writes[3].pImageInfo = nullptr;
	writes[3].pTexelBufferView = nullptr;

	vkUpdateDescriptorSets(
		device.getLogicalDevice(),
		static_cast<uint32_t>(writes.size()),
		writes.data(),
		0, nullptr
	);
}

void	DescriptorSet::_createUniformBuffers(Device& device) {
	// Camera and texture are dynamically updated.
	VkDeviceSize	buffer_size = sizeof(UniformBufferObject);

	// Create the buffer and allocate memory
	_ubo.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	// Map it to allow CPU to write on it
	_ubo.map(device.getLogicalDevice(), buffer_size);
}

/**
 * @brief	Initiate uniform buffer.
*/
void	DescriptorSet::_initUniformBuffer(
	const UniformBufferObject::Light& light
) noexcept {
	UniformBufferObject	ubo{};
	ubo.light = light;

	ubo.projector.proj = 
		scop::orthographic(1, 20, 1, 20, 1, 1000);
		//scop::perspective(45, 1, 1, 1000);
	ubo.projector.view = scop::lookAt(
		scop::Vect3(80, 20, 80),
		scop::Vect3(80, 0, 80),
		scop::normalize(scop::Vect3(1, 0.02, 0.0)));
	ubo.projector.proj[5] *= -1;

	_ubo.copyFrom(&ubo, sizeof(UniformBufferObject));
}

/**
 * Update the camera part of the uniform buffer.
*/
void	DescriptorSet::_updateCamera(
	VkExtent2D extent,
	const vox::Player& player
) {
	UniformBufferObject::Camera	camera{};

	// Define camera view
	camera.view = scop::lookAtDir(
		player.getPosition(),
		player.getEyeDir(),
		scop::Vect3(0.0f, 1.0f, 0.0f)
	);

	// Define persp. projection transformation
	camera.proj = scop::perspective(
		scop::math::radians(70.0f),
		extent.width / static_cast<float>(extent.height),
		0.1f,
		1000.0f
	);
	// Invert y axis (because y axis is inverted in Vulkan)
	camera.proj[5] *= -1;

	// Copy to uniform buffer
	_ubo.copyFrom(
		&camera,
		sizeof(UniformBufferObject::Camera),
		offsetof(UniformBufferObject, camera)
	);
}

} // namespace graphics
} // namespace scop