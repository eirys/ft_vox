/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 08:58:40 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 20:26:21 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <cstddef> // uint32_t
# include <cassert> // assert

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

	constexpr uint32_t	getPixelSize() const noexcept {
		switch (format) {

			// uint
			case VK_FORMAT_R8_UINT:
				return sizeof(uint8_t);

			case VK_FORMAT_R16_UINT:
			case VK_FORMAT_R8G8_UINT:
				return sizeof(uint16_t);

			case VK_FORMAT_R8G8B8_UINT:
			case VK_FORMAT_B8G8R8_UINT:
				return sizeof(uint8_t) * 3;

			case VK_FORMAT_R32_UINT:
			case VK_FORMAT_R16G16_UINT:
			case VK_FORMAT_R8G8B8A8_UINT:
			case VK_FORMAT_B8G8R8A8_UINT:
				return sizeof(uint32_t);

			case VK_FORMAT_R16G16B16_UINT:
				return sizeof(uint16_t) * 3;

			case VK_FORMAT_R64_UINT:
			case VK_FORMAT_R32G32_UINT:
			case VK_FORMAT_R16G16B16A16_UINT:
				return sizeof(uint64_t);

			case VK_FORMAT_R32G32B32_UINT:
				return sizeof(uint32_t) * 3;

			case VK_FORMAT_R64G64_UINT:
			case VK_FORMAT_R32G32B32A32_UINT:
				return sizeof(uint64_t) * 2;

			// sfloat
			case VK_FORMAT_R8G8B8A8_SRGB:
				return sizeof(uint32_t);

			default:
				assert(false);
				return 0;
		}
	}
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
	/*                                    DATA                                   */
	/* ========================================================================= */

	VkImage				_image;
	VkDeviceMemory		_memory;
	VkImageView			_view;

	ImageMetaData		_metadata;

}; // class ImageBuffer

} // namespace scop::gfx