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

	void		init(
		Device& device,
		const std::vector<DescriptorSetPtr>& sets);
	void		destroy(Device& device);
	void		add(
		Device& device,
		const std::vector<DescriptorSetPtr>& sets);

	/* ========================================================================= */

	const std::vector<VkDescriptorSetLayout>&	getLayouts() const noexcept;
	VkDescriptorPool							getPool() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkDescriptorPool					_pool;
	std::vector<VkDescriptorSetLayout>	_layouts;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void		_createDescriptorPool(
		Device& device,
		const std::vector<DescriptorSetPtr>& set);
	void		_allocateSets(
		Device& device,
		const std::vector<DescriptorSetPtr>& set);

}; // class DescriptorPool

} // namespace scop::graphics