/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_pipeline.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 21:11:40 by etran             #+#    #+#             */
/*   Updated: 2023/11/25 02:25:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <memory>

namespace scop::core {
class Device;
}

namespace scop::gfx {

class CommandBuffer;
class DescriptorSet;

class ComputePipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using DescriptorSetPtr = std::shared_ptr<DescriptorSet>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~ComputePipeline() = default;

	/* ========================================================================= */

	virtual void		init(scop::core::Device& device) = 0;
	virtual void		assemble(
		scop::core::Device& device,
		VkComputePipelineCreateInfo& info) = 0;
	virtual void		destroy(scop::core::Device& device) = 0;
	virtual void		compute(
		scop::core::Device& device,
		VkPipelineLayout layout,
		CommandBuffer& command_buffer) = 0;

	/* ========================================================================= */

	VkPipeline			getPipeline() const noexcept;
	DescriptorSetPtr	getDescriptor() const noexcept;

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkPipeline			_pipeline;

	DescriptorSetPtr	_descriptor;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ComputePipeline() = default;
	ComputePipeline(ComputePipeline&& other) = default;
	ComputePipeline& operator=(ComputePipeline&& other) = default;

	ComputePipeline(const ComputePipeline& other) = delete;
	ComputePipeline& operator=(const ComputePipeline& other) = delete;

}; // class ComputePipeline

} // namespace scop::gfx