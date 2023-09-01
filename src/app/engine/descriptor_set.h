/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:08:24 by etran             #+#    #+#             */
/*   Updated: 2023/09/01 15:29:33 by etran            ###   ########.fr       */
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

namespace scop::graphics {

class Device;

class DescriptorSet {
public:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	struct DescriptorSizes {
		uint32_t	uniform_buffer;
		uint32_t	combined_image_sampler;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual void			init(Device& device) = 0;

	void					destroy(Device& device);
	void					setDescriptors(VkDescriptorSet set) noexcept;

	/* ========================================================================= */

	VkDescriptorSetLayout	getLayout() const noexcept;
	VkDescriptorSet			getSet() const noexcept;
	const DescriptorSizes&	getPoolSizes() const noexcept;
	uint32_t				getSetIndex() const noexcept;

protected:
	/* ========================================================================= */
	/*                               STATIC MEMBERS                              */
	/* ========================================================================= */

	static uint32_t			_descriptor_count;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	const uint32_t			_index;

	VkDescriptorSetLayout	_layout;
	VkDescriptorSet			_set = VK_NULL_HANDLE;
	DescriptorSizes			_writes_sizes;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	DescriptorSet();

	virtual ~DescriptorSet() = default;

	DescriptorSet(DescriptorSet&& other) = delete;
	DescriptorSet(const DescriptorSet& other) = delete;
	DescriptorSet&	operator=(DescriptorSet&& other) = delete;
	DescriptorSet&	operator=(const DescriptorSet& other) = delete;

}; // class DescriptorSet

} // namespace scop::graphics