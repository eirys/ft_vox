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

#include "utils.h"

namespace scop::graphics {

using DescriptorPtr = DescriptorSet::DescriptorPtr;

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
	_descriptor_infos.emplace_back(std::make_shared<ImageInfo>(image_info));
	++_writes_sizes.combined_image_sampler;
}

void	DescriptorSet::addDescriptor(const BufferInfo& buffer_info) {
	_descriptor_infos.emplace_back(std::make_shared<BufferInfo>(buffer_info));
	++_writes_sizes.uniform_buffer;
}

/* ========================================================================== */

VkDescriptorSetLayout	DescriptorSet::getLayout() const noexcept {
	return _layout;
}

VkDescriptorSet	DescriptorSet::getSet() const noexcept {
	return _set;
}

DescriptorSet::DescriptorSizes	DescriptorSet::getPoolSizes() const noexcept {
	return _writes_sizes;
}

const Buffer& DescriptorSet::getBuffer() const noexcept {
	return _buffer;
}

const std::vector<DescriptorPtr>&	DescriptorSet::getInfos() const noexcept {
	return _descriptor_infos;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	DescriptorSet::_createLayout(Device& device) {
	std::vector<VkDescriptorSetLayoutBinding>	bindings;
	bindings.reserve(_descriptor_infos.size());

	for (const auto& info: _descriptor_infos) {
		VkDescriptorSetLayoutBinding binding{};
		binding.binding = info->binding;
		binding.descriptorCount = 1;
		binding.descriptorType = info->type;
		binding.stageFlags = info->stage;

		bindings.emplace_back(binding);
	}

	VkDescriptorSetLayoutCreateInfo	layout_info{};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device.getLogicalDevice(), &layout_info, nullptr, &_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}
	LOG("Created layouts");
}

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

} // namespace scop::graphics