/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:18 by etran             #+#    #+#             */
/*   Updated: 2023/06/19 14:42:46 by etran            ###   ########.fr       */
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

# include "device.h"
# include "texture_sampler.h"
# include "uniform_buffer_object.h"
# include "player.h"

# define TEXTURE_SAMPLER_COUNT 16

namespace scop {
namespace graphics {
class Engine;

class DescriptorSet {
public:

	friend Engine;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	DescriptorSet() = default;
	DescriptorSet(DescriptorSet&& other) = default;
	~DescriptorSet() = default;

	DescriptorSet(const DescriptorSet& other) = delete;
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

private:
	/* ========================================================================= */
	/*                                  TYPEDEF                                  */
	/* ========================================================================= */

	typedef	std::chrono::high_resolution_clock::time_point	time_point;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkDescriptorSetLayout	vk_descriptor_set_layout;
	VkDescriptorPool		vk_descriptor_pool;
	VkDescriptorSet			vk_descriptor_sets;

	VkBuffer				uniform_buffers;
	VkDeviceMemory			uniform_buffers_memory;
	void*					uniform_buffers_mapped;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					createDescriptorSetLayout(
		Device& device
	);
	void					createDescriptorPool(Device& device, uint32_t fif);
	void					createDescriptorSets(
		Device& device,
		TextureSampler& texture_sampler,
		uint32_t frames_in_flight
	);
	void					createUniformBuffers(Device& device);
	void					initUniformBuffer(
		const UniformBufferObject::Light& light
	) noexcept;

	void					updateCamera(
		VkExtent2D extent,
		const vox::Player& player
	);

}; // class DescriptorSet

} // namespace graphics
} // namespace scop