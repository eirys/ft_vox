/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:08:24 by etran             #+#    #+#             */
/*   Updated: 2023/11/18 20:21:54 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <vector> // std::vector
# include <memory> // std::shared_ptr

namespace scop::core {
class Device;
}

namespace scop::gfx {

/* ========================================================================= */
/*                               HELPER OBJECTS                              */
/* ========================================================================= */

struct DescriptorSizes {
	uint32_t	uniform_buffer = 0;
	uint32_t	combined_image_sampler = 0;
	uint32_t	storage_image = 0;
	uint32_t	storage_buffer = 0;
};

/**
 * @brief Descriptor set handle.
*/
class DescriptorSet {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual void			init(scop::core::Device& device) = 0;

	void					destroy(scop::core::Device& device);
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

} // namespace scop::gfx