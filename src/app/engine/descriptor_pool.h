/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:18 by etran             #+#    #+#             */
/*   Updated: 2023/08/20 10:00:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <vector> // std::vector
# include <memory> // std::shared_ptr

namespace scop::gfx {
class DescriptorSet;
}

namespace scop::core {

class Device;

class DescriptorPool {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using DescriptorSetPtr = std::shared_ptr<scop::gfx::DescriptorSet>;

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

	void				init(Device& device, const std::vector<DescriptorSetPtr>& sets);
	void				destroy(Device& device);

	/* ========================================================================= */

	VkDescriptorPool	getPool() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkDescriptorPool	_pool;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void				_createDescriptorPool(
		Device& device,
		const std::vector<DescriptorSetPtr>& sets);
	void				_allocateSets(
		Device& device,
		const std::vector<DescriptorSetPtr>& sets);

}; // class DescriptorPool

} // namespace scop::gfx