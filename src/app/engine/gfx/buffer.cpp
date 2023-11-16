/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 06:44:38 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 22:51:27 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buffer.h"
#include "device.h"

#include <stdexcept> // std::runtime_error
#include <cstring> // memcpy

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Initialize buffer, allocate memory and bind it to buffer
*/
void	Buffer::init(
	scop::core::Device& device,
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkSharingMode sharing_mode
) {
	// Create buffer instance
	VkBufferCreateInfo	buffer_info{};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = size;
	buffer_info.usage = usage;
	buffer_info.sharingMode = sharing_mode;

	if (vkCreateBuffer(device.getLogicalDevice(), &buffer_info, nullptr, &_buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer");
	}

	// Allocate memory for buffer
	VkMemoryRequirements	mem_requirements;
	vkGetBufferMemoryRequirements(
		device.getLogicalDevice(),
		_buffer,
		&mem_requirements
	);

	VkMemoryAllocateInfo	alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = device.findMemoryType(
		mem_requirements.memoryTypeBits,
		properties
	);

	if (vkAllocateMemory(device.getLogicalDevice(), &alloc_info, nullptr, &_memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory");
	}

	// Bind memory to instance
	vkBindBufferMemory(device.getLogicalDevice(), _buffer, _memory, 0);
}

/**
 * @brief Destroy buffer and free memory
*/
void	Buffer::destroy(VkDevice device) {
	vkDestroyBuffer(device, _buffer, nullptr);
	vkFreeMemory(device, _memory, nullptr);
}

/* ========================================================================== */

/**
 * @brief Copies data to buffer.
 *
 * @note Buffer must be mapped before calling this function.
 *
 * @param data		Data to copy.
 * @param data_size	Size of data.
 * @param offset	Offset of data in buffer.
*/
void	Buffer::copyFrom(
	const void* data,
	std::size_t data_size,
	std::size_t offset
) noexcept {
	memcpy(reinterpret_cast<uint8_t*>(_data) + offset, data, data_size);
}

/**
 * @brief Sends a copy command to a command buffer.
 *
 * @param command_buffer	Command buffer to send command to.
 * @param src_buffer		Source buffer.
 * @param size				Size of data to copy. Default to VK_WHOLE_SIZE.
 * @param src_offset		Offset of data in src buffer. Default to 0.
 * @param dst_offset		Offset of data in dst buffer. Default to 0.
*/

void	Buffer::copyBuffer(
	VkCommandBuffer command_buffer,
	Buffer& src_buffer,
	VkDeviceSize size,
	VkDeviceSize src_offset,
	VkDeviceSize dst_offset
) noexcept {
	VkBufferCopy	copy_region{};
	copy_region.srcOffset = src_offset;
	copy_region.dstOffset = dst_offset;
	copy_region.size = size;

	vkCmdCopyBuffer(
		command_buffer,
		src_buffer.getBuffer(),
		_buffer,
		1, &copy_region
	);
}

/* ========================================================================== */

VkBuffer	Buffer::getBuffer() const noexcept {
	return _buffer;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

/**
 * @brief Maps buffer memory to CPU accessible memory.
 *
 * @param device	Vulkan device.
 * @param size		Size of memory to map. Default to VK_WHOLE_SIZE.
*/
void	Buffer::map(VkDevice device, VkDeviceSize size) {
	if (vkMapMemory(device, _memory, 0, size, 0, &_data) != VK_SUCCESS) {
		throw std::runtime_error("failed to map buffer memory");
	}
}

/**
 * @brief Unmaps buffer memory.
 *
 * @param device	Vulkan device.
*/
void	Buffer::unmap(VkDevice device) noexcept {
	vkUnmapMemory(device, _memory);
}

} // namespace scop::gfx