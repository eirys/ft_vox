/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 06:30:15 by etran             #+#    #+#             */
/*   Updated: 2023/12/26 17:41:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <cstddef> // std::size_t

namespace scop::core {
class Device;
}

namespace scop::gfx {

/**
 * @brief Wrapper class for VkBuffer and VkDeviceMemory
*/
class Buffer {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Buffer() = default;
	Buffer(Buffer&& other) = default;
	Buffer(const Buffer& other) = default;
	Buffer& operator=(Buffer&& other) = default;
	Buffer& operator=(const Buffer& other) = default;
	~Buffer() = default;

	/* ========================================================================= */

	void				init(
		scop::core::Device& device,
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkSharingMode sharing_mode = VK_SHARING_MODE_EXCLUSIVE);
	void				init(
		scop::core::Device& device,
		const std::vector<VkBufferCreateInfo>& buffer_infos,
		VkMemoryPropertyFlags properties);

	void				destroy(VkDevice device);

	/* ========================================================================= */

	void				map(VkDevice device, VkDeviceSize size = VK_WHOLE_SIZE);
	void				unmap(VkDevice device) noexcept;

	void				copyFrom(
		const void* data,
		std::size_t data_size,
		std::size_t offset = 0) noexcept;
	void				copyTo(
		void* data_dst,
		std::size_t data_size,
		std::size_t offset = 0) noexcept;
	void				copyBuffer(
		VkCommandBuffer command_buffer,
		Buffer& src_buffer,
		VkDeviceSize size = VK_WHOLE_SIZE,
		VkDeviceSize src_offset = 0,
		VkDeviceSize dst_offset = 0) noexcept;


	/* ========================================================================= */

	VkBuffer			getBuffer() const noexcept;
	void*				getMappedData() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkBuffer			_buffer;
	VkDeviceMemory		_memory;
	void*				_data;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void				_allocateBuffer(
		scop::core::Device& device,
		VkMemoryPropertyFlags properties);

}; // class Buffer

} // namespace scop::gfx