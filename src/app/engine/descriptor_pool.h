/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:18 by etran             #+#    #+#             */
/*   Updated: 2023/08/12 00:55:33 by etran            ###   ########.fr       */
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

namespace scop::graphics {

class Device;
class TextureHandler;

class DescriptorPool {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	DescriptorPool() = default;
	~DescriptorPool() = default;

	DescriptorPool(DescriptorPool&& other) = delete;
	DescriptorPool(const DescriptorPool& other) = delete;
	DescriptorPool&	operator=(const DescriptorPool& other) = delete;
	DescriptorPool&	operator=(DescriptorPool&& other) = delete;

	/* ========================================================================= */

	void					initLayout(Device& device);
	void					initSets(
		Device& device,
		TextureHandler& texture_handler,
		const ::scop::UniformBufferObject::Light& light
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
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkDescriptorSetLayout	_layout;
	VkDescriptorPool		_pool;
	VkDescriptorSet			_set;

	Buffer					_ubo;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_createDescriptorPool(Device& device);
	void					_createDescriptorSets(
		Device& device,
		TextureHandler& texture_handler);
	void					_createUniformBuffers(Device& device);
	void					_initUniformBuffer(
		const UniformBufferObject::Light& light
	) noexcept;

	void					_updateCamera(
		VkExtent2D extent,
		const vox::Player& player
	);

}; // class DescriptorPool

} // namespace scop::graphics