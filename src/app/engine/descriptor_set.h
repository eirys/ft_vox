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
class TextureHandler;

class DescriptorSet {
public:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	struct DescriptorSizes {
		std::size_t	uniform_buffer;
		std::size_t	combined_image_sampler;
	};

	struct DescriptorWrites {
		std::vector<VkWriteDescriptorSet>		writes_data;
		std::vector<VkDescriptorBufferInfo>		buffer_infos;
		std::vector<VkDescriptorImageInfo>		image_infos;
	};

	struct Descriptor {
		VkDescriptorType	type;
		VkShaderStageFlags	stage;
		uint32_t			binding;
	};

	struct BufferInfo: public Descriptor {
		VkDescriptorType	type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		VkDeviceSize		offset;
		VkDeviceSize		range;
	};

	struct ImageInfo: public Descriptor {
		VkDescriptorType	type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
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

	void	init(
		Device& device
		// TextureHandler& texture_handler,
		// const ::scop::UniformBufferObject& ubo);
		);
	void	destroy(Device& device);
	void	update(const ::scop::UniformBufferObject& ubo) noexcept;

	void	addDescriptor(const ImageInfo& image_info);
	void	addDescriptor(const BufferInfo& buffer_info);
	void	removeWrites();

	/* ========================================================================= */

	VkDescriptorSetLayout						getLayout() const noexcept;
	VkDescriptorSet								getSet() const noexcept;
	DescriptorSizes								getPoolSizes() const noexcept;
	const std::vector<VkWriteDescriptorSet>&	getWrites() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkDescriptorSetLayout						_layout;
	VkDescriptorSet								_set;
	Buffer										_buffer;

	DescriptorWrites							_writes;
	std::vector<DescriptorPtr>					_descriptor_infos;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void				_createLayout(Device& device);
	void				_createWrites(TextureHandler& texture_handler);
	void				_createUniformBuffers(Device& device);
	// void				_initUniformBuffer(
		// const ::scop::UniformBufferObject& ubo) noexcept;
	// void				_updateUniformBuffer(
	// 	const ::scop::UniformBufferObject& ubo);

}; // class DescriptorSet

} // namespace scop::graphics