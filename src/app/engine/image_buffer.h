/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 08:58:40 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:09:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

// Std
# include <cstddef> // std::size_t

namespace scop::graphics {

class Device;

/**
 * @brief Wrapper class for VkImage, VkDeviceMemory and VkImageView
*/
class ImageBuffer final {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ImageBuffer() = default;
	ImageBuffer(ImageBuffer&& other) = default;
	ImageBuffer(const ImageBuffer& other) = default;
	ImageBuffer& operator=(ImageBuffer&& other) = default;
	ImageBuffer& operator=(const ImageBuffer& other) = default;
	~ImageBuffer() = default;

	/* ========================================================================= */

	void				initImage(
		Device& device,
		uint32_t width,
		uint32_t height,
		VkFormat format,
		VkImageUsageFlags usage,
		VkSampleCountFlagBits sample_count,
		uint32_t mip_level = 1,
		uint32_t layers = 1,
		VkImageCreateFlags flags = 0,
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
		uint32_t depth = 1,
		VkImageType type = VK_IMAGE_TYPE_2D);

	void				initView(
		Device& device,
		VkFormat image_format,
		VkImageAspectFlags aspect_flags,
		VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D,
		uint32_t mip_count = 1,
		uint32_t layer_count = 1);

	void				destroy(Device& device);

	/* ========================================================================= */

	void				setLayout(
		VkCommandBuffer command_buffer,
		VkImageLayout old_layout,
		VkImageLayout new_layout,
		VkAccessFlags src_access_mask,
		VkAccessFlags dst_access_mask,
		VkPipelineStageFlags src_stage_mask,
		VkPipelineStageFlags dst_stage_mask,
		const VkImageSubresourceRange& subresource_range
	);

	void				copyFrom(
		VkCommandBuffer command_buffer,
		VkBuffer src_buffer,
		uint32_t image_width,
		uint32_t image_height,
		uint32_t image_count = 1,
		uint32_t layer_count = 1,
		uint32_t layer_size = 0,
		uint32_t pixel_size = sizeof(uint32_t)
	);

	void				generateMipmap(
		VkCommandBuffer command_buffer,
		Device& device,
		uint32_t image_width,
		uint32_t image_height,
		VkFormat image_format,
		uint32_t mip_count,
		uint32_t layer_count
	);

	/* ========================================================================= */

	VkImage				getImage() const noexcept;
	VkImageView			getView() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkImage				_image;
	VkDeviceMemory		_memory;
	VkImageView			_view;

}; // class ImageBuffer


} // namespace vox