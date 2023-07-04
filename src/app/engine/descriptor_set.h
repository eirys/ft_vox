/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:18 by etran             #+#    #+#             */
/*   Updated: 2023/07/04 10:10:13 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

// Std
# include <chrono> // std::chrono

# include "buffer.h"
# include "uniform_buffer_object.h"
# include "player.h"

# define TEXTURE_SAMPLER_COUNT 16

namespace scop {
namespace graphics {
class Device;
class TextureSampler;

class DescriptorSet {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	DescriptorSet() = default;
	~DescriptorSet() = default;

	DescriptorSet(DescriptorSet&& other) = delete;
	DescriptorSet(const DescriptorSet& other) = delete;
	DescriptorSet&	operator=(const DescriptorSet& other) = delete;
	DescriptorSet&	operator=(DescriptorSet&& other) = delete;

	/* ========================================================================= */

	void					initLayout(
		Device& device
	);
	void					initSets(
		Device& device,
		TextureSampler& texture_sampler,
		const UniformBufferObject::Light& light
	);
	void					destroy(Device& device);
	void					updateUniformBuffer(
		VkExtent2D extent,
		const vox::Player& player
	);

	/* ========================================================================= */

	VkDescriptorSetLayout	getLayout() const noexcept;
	VkDescriptorSet			getSet() const noexcept;

private:
	/* ========================================================================= */
	/*                                  TYPEDEF                                  */
	/* ========================================================================= */

	typedef	std::chrono::high_resolution_clock::time_point	time_point;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkDescriptorSetLayout	_layout;
	VkDescriptorPool		_pool;
	VkDescriptorSet			_set;

	Buffer					_ubo;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_createDescriptorPool(Device& device, uint32_t count);
	void					_createDescriptorSets(
		Device& device,
		TextureSampler& texture_sampler,
		uint32_t count
	);
	void					_createUniformBuffers(Device& device);
	void					_initUniformBuffer(
		const UniformBufferObject::Light& light
	) noexcept;

	void					_updateCamera(
		VkExtent2D extent,
		const vox::Player& player
	);

}; // class DescriptorSet

} // namespace graphics
} // namespace scop