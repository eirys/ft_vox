/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 00:55:41 by etran             #+#    #+#             */
/*   Updated: 2023/05/16 14:53:44 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEVICE_HPP
# define DEVICE_HPP

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

namespace scop {
namespace graphics {

class GraphicsPipeline;
class RenderTarget;
class RenderTargetResources;
class DescriptorSet;
class CommandBuffer;
class TextureSampler;
class VertexInput;

class Device {
public:

	friend class GraphicsPipeline;
	friend class RenderTarget;
	friend class RenderTargetResources;
	friend class DescriptorSet;
	friend class CommandBuffer;
	friend class TextureSampler;
	friend class VertexInput;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Device() = default;
	Device(Device&& other) = default;
	~Device() = default;

	Device(const Device& other) = delete;
	Device& operator=(const Device& other) = delete;

	/* ========================================================================= */

	void							init(Window& window, VkInstance instance);
	void							destroy(VkInstance instance);
	QueueFamilyIndices				findQueueFamilies() const;
	uint32_t						findMemoryType(
		uint32_t type_filter,
		VkMemoryPropertyFlags properties
	) const;
	void							createImage(
		uint32_t width,
		uint32_t height,
		uint32_t mip_level,
		VkSampleCountFlagBits num_samples,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage& image,
		VkDeviceMemory& image_memory
	);
	void							createBuffer(
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& buffer_memory
	);

private:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	const std::vector<const char*>	device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkSurfaceKHR					vk_surface;

	VkPhysicalDevice				physical_device = VK_NULL_HANDLE;
	VkDevice						logical_device;

	VkQueue							graphics_queue;
	VkQueue							present_queue;

	VkSampleCountFlagBits			msaa_samples;


	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void							createSurface(VkInstance instance, Window& window);
	void							pickPhysicalDevice(VkInstance vk_instance);
	void							createLogicalDevice();

	/* ========================================================================= */

	VkSampleCountFlagBits			getMaxUsableSampleCount() const;
	bool							checkDeviceExtensionSupport(
		const VkPhysicalDevice& device
	);
	bool							isDeviceSuitable(
		const VkPhysicalDevice& device
	);

}; // class Device

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

inline VkFormat	findSupportedFormat(
	VkPhysicalDevice physical_device,
	const std::vector<VkFormat>& candidates,
	VkImageTiling tiling,
	VkFormatFeatureFlags features
);

inline VkFormat	findDepthFormat(
	VkPhysicalDevice physical_device
);

inline bool	hasStencilCompotent(VkFormat format) noexcept;

} // namespace graphics
} // namespace scop

#endif