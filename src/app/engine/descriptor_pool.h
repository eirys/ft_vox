/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:18 by etran             #+#    #+#             */
/*   Updated: 2023/08/14 12:35:44 by etran            ###   ########.fr       */
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
# include <memory> // std::shared_ptr

namespace scop::graphics {

class Device;
class TextureHandler;
class DescriptorSet;

class DescriptorPool {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using DescriptorSetPtr = std::shared_ptr<DescriptorSet>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	DescriptorPool() = default;
	~DescriptorPool() = default;

	DescriptorPool(DescriptorPool&& other) = delete;
	DescriptorPool(const DescriptorPool& other) = delete;
	DescriptorPool&	operator=(DescriptorPool&& other) = delete;
	DescriptorPool&	operator=(const DescriptorPool& other) = delete;

	/* ========================================================================= */

	void		init(Device& device);
	void		add(DescriptorSetPtr set);
	void		destroy(Device& device);

	/* ========================================================================= */

	const std::vector<VkDescriptorSetLayout>&	getLayouts() const noexcept;
	VkDescriptorPool							getPool() const noexcept;
	const std::vector<DescriptorSetPtr>&		getDescriptors() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkDescriptorPool					_pool;
	std::vector<VkDescriptorSetLayout>	_layouts;

	std::vector<DescriptorSetPtr>		_descriptors;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void		_createDescriptorPool(Device& device);
	void		_allocateSets(Device& device);
	void		_createWrites(Device& device)

}; // class DescriptorPool

} // namespace scop::graphics