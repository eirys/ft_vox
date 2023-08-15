/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:08:24 by etran             #+#    #+#             */
/*   Updated: 2023/08/15 19:07:55 by etran            ###   ########.fr       */
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
		Device& device,
		TextureHandler& texture_handler,
		const ::scop::UniformBufferObject& ubo);
	void	destroy(Device& device);
	void	update(const ::scop::UniformBufferObject& ubo) noexcept;
	void	removeWrites(Device& device);

	// void	update(const ::scop::UniformBufferObject::Light& ubo) noexcept;
	// void	update(const ::scop::UniformBufferObject::Camera& camera) noexcept;
	// void	update(const ::scop::UniformBufferObject::Projector& ubo) noexcept;

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

	struct {
		std::vector<VkWriteDescriptorSet>		writes_data;
		std::vector<VkDescriptorBufferInfo>		buffer_infos;
		std::vector<VkDescriptorImageInfo>		image_infos;
	}											_writes;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void				_createLayout(Device& device);
	void				_createWrites(TextureHandler& texture_handler);
	void				_createUniformBuffers(Device& device);
	void				_initUniformBuffer(
		const ::scop::UniformBufferObject& ubo) noexcept;
	void				_updateUniformBuffer(
		const ::scop::UniformBufferObject& ubo);

}; // class DescriptorSet

} // namespace scop::graphics