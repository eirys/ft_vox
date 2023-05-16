/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:39:20 by etran             #+#    #+#             */
/*   Updated: 2023/05/16 14:26:10 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DESCRIPTOR_SET_HPP
# define DESCRIPTOR_SET_HPP

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

# include "uniform_buffer_object.hpp"
# include "app.hpp"

namespace scop {
namespace graphics {

class DescriptorSet {
public:

	friend class GraphicsPipeline;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	DescriptorSet() = default;
	DescriptorSet(DescriptorSet&& other) = default;
	~DescriptorSet() = default;

	DescriptorSet(const DescriptorSet& other) = delete;
	DescriptorSet&	operator=(DescriptorSet&& other) = delete;

	/* ========================================================================= */

	void					init(Device& device, TextureSampler& texture_sampler);
	void					destroy(Device& device);
	void					updateUniformBuffer(const scop::App::Input& input);

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

	void					createDescriptorSetLayout(Device& device);
	void					createDescriptorPool(Device& device, uint32_t fif);
	void					createDescriptorSets(
		Device& device,
		TextureSampler& texture_sampler,
		uint32_t fif
	);
	void					createUniformBuffers(Device& device);
	void					initUniformBuffer() noexcept;

	void					updateVertexPart(
		time_point current_time
	);
	void					updateFragmentPart(
		time_point current_time
	);

}; // class DescriptorSet

} // namespace graphics
} // namespace scop

#endif