/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_set.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 12:08:24 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 12:55:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <vector> // std::vector
# include <memory> // std::shared_ptr

# include "glsl/decl.glslh"

namespace scop::core {
class Device;
}

namespace scop::gfx {

/* ========================================================================= */
/*                               HELPER OBJECTS                              */
/* ========================================================================= */

enum class DescriptorType: uint8_t {
	UNIFORM_BUFFER,
	COMBINED_IMAGE_SAMPLER,
	SAMPLED_IMAGE,
	STORAGE_IMAGE,
	STORAGE_BUFFER
};

struct DescriptorSizes {
	uint32_t	uniform_buffer = 0;
	uint32_t	combined_image_sampler = 0;
	uint32_t	storage_image = 0;
	uint32_t	storage_buffer = 0;
	uint32_t	sampled_image = 0;
};

/**
 * @brief Descriptor set handle.
*/
class DescriptorSet {
public:
	/* ========================================================================= */
	/*                                   ENUMS                                   */
	/* ========================================================================= */

	enum class DescriptorIndex: uint32_t {
		Scene = SCENE_SET,
		Shadows = SHADOW_SET,
		Culling = CULLING_SET,

		First = Scene,
		Last = Culling
	};

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
	/*                                    DATA                                   */
	/* ========================================================================= */

	const uint32_t			_index;

	VkDescriptorSetLayout	_layout;
	VkDescriptorSet			_set = VK_NULL_HANDLE;
	DescriptorSizes			_writes_sizes = {};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	DescriptorSet(const DescriptorIndex index);

	virtual ~DescriptorSet() = default;

	DescriptorSet() = delete;
	DescriptorSet(DescriptorSet&& other) = delete;
	DescriptorSet(const DescriptorSet& other) = delete;
	DescriptorSet&	operator=(DescriptorSet&& other) = delete;
	DescriptorSet&	operator=(const DescriptorSet& other) = delete;

	/* ========================================================================= */

	VkDescriptorSetLayoutBinding	createLayoutBinding(
		DescriptorType type,
		VkShaderStageFlags shader_stage,
		uint32_t binding_index,
		uint32_t count = 1);

	VkWriteDescriptorSet			createWriteDescriptorSet(
		DescriptorType type,
		void* descriptor_info,
		uint32_t binding_index,
		uint32_t count = 1) const;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_fillDescriptorType(
		DescriptorType type,
		void* descriptor_info,
		VkWriteDescriptorSet& write_descriptor_set) const;

}; // class DescriptorSet

} // namespace scop::gfx