/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:18 by etran             #+#    #+#             */
/*   Updated: 2024/01/05 12:39:57 by etran            ###   ########.fr       */
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

class DescriptorSet;

class DescriptorPool final {
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

	void				init(
		scop::core::Device& device,
		const std::vector<DescriptorSetPtr>& sets);
	void				destroy(scop::core::Device& device);

	/* ========================================================================= */

	static VkDescriptorPool	getPool() noexcept;

private:
	/* ========================================================================= */
	/*                               STATIC MEMBERS                              */
	/* ========================================================================= */

	static VkDescriptorPool	_pool;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void				_createDescriptorPool(
		scop::core::Device& device,
		const std::vector<DescriptorSetPtr>& sets);
	void				_allocateSets(
		scop::core::Device& device,
		const std::vector<DescriptorSetPtr>& sets);

}; // class DescriptorPool

} // namespace scop::gfx