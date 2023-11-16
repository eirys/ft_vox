/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 08:58:40 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 22:38:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <cstddef> // uint32_t

namespace scop::core {
class Device;
}

namespace scop::gfx {

/* ========================================================================== */
/*                                HELPER CLASS                                */
/* ========================================================================== */

struct ImageMetaData {
	VkFormat			format;
	uint32_t			layer_count;
	uint32_t			width;
	uint32_t			height;
	uint32_t			depth = 1;
	uint32_t			mip_count = 1;


	VkFormat			getFormat() const noexcept { return format; }
	uint32_t			getWidth() const noexcept { return width; }
	uint32_t			getHeight() const noexcept { return height; }
	uint32_t			getDepth() const noexcept { return depth; }
	uint32_t			getLayerCount() const noexcept { return layer_count; }
	uint32_t			getMipCount() const noexcept { return mip_count; }
};

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
		scop::core::Device& device,
		VkImageUsageFlags usage,
		VkSampleCountFlagBits sample_count,
		VkImageCreateFlags flags = 0,
		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
		VkImageType type = VK_IMAGE_TYPE_2D);

	void				initView(
		scop::core::Device& device,
		VkImageAspectFlags aspect_flags,
		VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D);

	void				destroy(scop::core::Device& device);

	/* ========================================================================= */

	void				setLayout(
		VkCommandBuffer command_buffer,
		VkImageLayout old_layout,
		VkImageLayout new_layout,
		VkAccessFlags src_access_mask,
		VkAccessFlags dst_access_mask,
		VkPipelineStageFlags src_stage_mask,
		VkPipelineStageFlags dst_stage_mask,
		const VkImageSubresourceRange& subresource_range);

	void				copyFrom(
		VkCommandBuffer command_buffer,
		VkBuffer src_buffer,
		uint32_t layer_size = 0);

	void				generateMipmap(
		VkCommandBuffer command_buffer,
		scop::core::Device& device);

	/* ========================================================================= */

	void					setMetaData(const ImageMetaData& metadata);

	/* ========================================================================= */

	VkImage					getImage() const noexcept;
	VkImageView				getView() const noexcept;
	const ImageMetaData&	getMetaData() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkImage				_image;
	VkDeviceMemory		_memory;
	VkImageView			_view;

	ImageMetaData		_metadata;

}; // class ImageBuffer

} // namespace scop::gfx