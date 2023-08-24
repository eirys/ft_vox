/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:08:24 by etran             #+#    #+#             */
/*   Updated: 2023/08/22 21:42:18 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif
# include <GLFW/glfw3.h>

// Std
# include <vector> // std::vector
# include <memory> // std::unique_ptr

# include "buffer.h"

namespace scop {
struct UniformBufferObject;
}

namespace scop::graphics {

class Device;
class DescriptorPool;

class DescriptorSet {

	friend DescriptorPool;

public:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	struct DescriptorSizes {
		uint32_t	uniform_buffer;
		uint32_t	combined_image_sampler;
	};

	struct Descriptor {
		Descriptor(VkDescriptorType type): type(type) {}
		virtual ~Descriptor() = default;

		VkDescriptorType	type;
		VkShaderStageFlags	stage;
		uint32_t			binding;
	};

	struct BufferInfo final: public Descriptor {
		BufferInfo(): Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {}

		VkDeviceSize		offset;
		VkDeviceSize		range;
	};

	struct ImageInfo final: public Descriptor {
		ImageInfo(): Descriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {}

		VkSampler			sampler;
		VkImageView			view;
		VkImageLayout		layout;
	};

	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using DescriptorPtr = std::shared_ptr<Descriptor>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	DescriptorSet() = default;
	~DescriptorSet() = default;
	DescriptorSet(DescriptorSet&& other) = default;
	DescriptorSet&	operator=(DescriptorSet&& other) = default;

	DescriptorSet(const DescriptorSet& other) = delete;
	DescriptorSet&	operator=(const DescriptorSet& other) = delete;

	/* ========================================================================= */

	void	init(Device& device);
	void	destroy(Device& device);
	void	update(const ::scop::UniformBufferObject& ubo) noexcept;

	void	addDescriptor(const ImageInfo& image_info);
	void	addDescriptor(const BufferInfo& buffer_info);

	/* ========================================================================= */

	VkDescriptorSetLayout				getLayout() const noexcept;
	VkDescriptorSet						getSet() const noexcept;
	DescriptorSizes						getPoolSizes() const noexcept;
	const Buffer&						getBuffer() const noexcept;
	const std::vector<DescriptorPtr>&	getInfos() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkDescriptorSetLayout				_layout;
	VkDescriptorSet						_set;
	Buffer								_buffer;

	DescriptorSizes						_writes_sizes;
	std::vector<DescriptorPtr>			_descriptor_infos;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void								_createLayout(Device& device);
	void								_createUniformBuffers(Device& device);

}; // class DescriptorSet

} // namespace scop::graphics