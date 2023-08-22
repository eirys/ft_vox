/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:33:52 by etran             #+#    #+#             */
/*   Updated: 2023/08/22 21:42:09 by etran            ###   ########.fr       */
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
	Device& device
	// TextureHandler& texture_handler,
	// const ::scop::UniformBufferObject& ubo
) {
	_createLayout(device);
	_createUniformBuffers(device);
	// _createWrites(texture_handler);
	// _initUniformBuffer(ubo);
}

void	DescriptorSet::destroy(Device& device) {
	_buffer.unmap(device.getLogicalDevice());
	_buffer.destroy(device.getLogicalDevice());

	vkDestroyDescriptorSetLayout(
		device.getLogicalDevice(),
		_layout,
		nullptr);
}

/**
 * @brief	Copies updated ubo to the uniform buffer.
*/
void	DescriptorSet::update(
	const ::scop::UniformBufferObject& ubo
) noexcept {
	_buffer.copyFrom(&ubo, sizeof(UniformBufferObject));
}

void	DescriptorSet::addDescriptor(const ImageInfo& image_info) {
	_descriptor_infos.emplace_back(std::make_shared<Descriptor>(image_info));
	// VkDescriptorImageInfo	image{};
	// image.sampler = image_info.sampler;
	// image.imageView = image_info.view;
	// image.imageLayout = image_info.layout;
	// const auto& info = _writes.image_infos.emplace_back(image);

	// VkWriteDescriptorSet	write{};
	// write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	// write.dstSet = _set;
	// write.dstBinding = image_info.binding;
	// write.dstArrayElement = 0;
	// write.descriptorCount = 1;
	// write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// write.pImageInfo = &info;
	// _writes.writes_data.emplace_back(write);
}

void	DescriptorSet::addDescriptor(const BufferInfo& buffer_info) {
	_descriptor_infos.emplace_back(std::make_shared<Descriptor>(buffer_info));
	// VkDescriptorBufferInfo	buffer{};
	// buffer.buffer = _buffer.getBuffer();
	// buffer.offset = buffer_info.offset;
	// buffer.range = buffer_info.range;
	// const auto& info = _writes.buffer_infos.emplace_back(buffer_info);

	// VkWriteDescriptorSet	write{};
	// write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	// write.dstSet = _set;
	// write.dstBinding = buffer_info.binding;
	// write.dstArrayElement = 0;
	// write.descriptorCount = 1;
	// write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	// write.pBufferInfo = &info;
	// _writes.writes_data.emplace_back(write);
}

/**
 * @brief	Destroys writes data after they have been used.
*/
void	DescriptorSet::removeWrites() {
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
		_writes.buffer_infos.size(), // 3,
		_writes.image_infos.size() }; // 2 };
}

const std::vector<VkWriteDescriptorSet>&	DescriptorSet::getWrites() const noexcept {
	return _writes.writes_data;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	DescriptorSet::_createLayout(Device& device) {
	std::vector<VkDescriptorSetLayoutBinding>	bindings;
	bindings.reserve(_descriptor_infos.size());

	for (uint32_t i = 0; i < bindings.size(); ++i) {
		bindings[i].binding = i;
		bindings[i].descriptorCount = 1;
		bindings[i].descriptorType = _descriptor_infos[i]->type;
		bindings[i].stageFlags = _descriptor_infos[i]->stage;
	}

// 	// Camera UBO layout
// 	VkDescriptorSetLayoutBinding	camera_binding{};
// 	camera_binding.binding = 0;
// 	camera_binding.descriptorCount = 1;
// 	camera_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
// 	camera_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

// 	// Sampler layout
// 	VkDescriptorSetLayoutBinding	sampler_binding{};
// 	sampler_binding.binding = 1;
// 	sampler_binding.descriptorCount = 1;
// 	sampler_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
// 	sampler_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

// 	// Light UBO layout
// 	VkDescriptorSetLayoutBinding	light_binding{};
// 	light_binding.binding = 2;
// 	light_binding.descriptorCount = 1;
// 	light_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
// 	light_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

// 	// Projector UBO layout
// 	VkDescriptorSetLayoutBinding	projector_binding{};
// 	projector_binding.binding = 3;
// 	projector_binding.descriptorCount = 1;
// 	projector_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
// 	projector_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


// 	// TODO 5
// 	std::array<VkDescriptorSetLayoutBinding, 5>	bindings = {
// 		camera_binding,
// 		sampler_binding,
// 		light_binding,
// 		projector_binding };

	VkDescriptorSetLayoutCreateInfo	layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device.getLogicalDevice(), &layout_info, nullptr, &_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}
}
/*
void	DescriptorSet::_createWrites(
	TextureHandler& texture_handler
) {
	_writes.buffer_infos.reserve(3);
	_writes.image_infos.reserve(2);

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
	VkDescriptorImageInfo	shadowmap_info{};
	shadowmap_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	shadowmap_info.imageView = shadowmap.getTextureBuffer().getView();
	shadowmap_info.sampler = shadowmap.getTextureSampler();

	_writes.writes_data.reserve(5);

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
	_writes.writes_data[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	_writes.writes_data[4].dstSet = _set;
	_writes.writes_data[4].dstBinding = 1;
	_writes.writes_data[4].dstArrayElement = 0;
	_writes.writes_data[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	_writes.writes_data[4].descriptorCount = 1;
	_writes.writes_data[4].pBufferInfo = nullptr;
	_writes.writes_data[4].pImageInfo = std::make_shared<VkDescriptorImageInfo>(shadowmap_info).get();
	_writes.writes_data[4].pTexelBufferView = nullptr;

	// vkUpdateDescriptorSets(
	// 	device.getLogicalDevice(),
	// 	static_cast<uint32_t>(_writes.size()),
	// 	_writes.data(),
	// 	0, nullptr
	// );
}
 */
void	DescriptorSet::_createUniformBuffers(Device& device) {
	VkDeviceSize	buffer_size = sizeof(UniformBufferObject);

	_buffer.init(
		device,
		buffer_size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	_buffer.map(device.getLogicalDevice(), buffer_size);
}

// /**
//  * @brief	Initiate uniform buffer.
// */
// void	DescriptorSet::_initUniformBuffer(
// 	const ::scop::UniformBufferObject& ubo
// ) noexcept {
// 	_buffer.copyFrom(&ubo, sizeof(UniformBufferObject));
// }

} // namespace scop::graphics