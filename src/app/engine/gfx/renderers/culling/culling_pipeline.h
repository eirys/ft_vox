/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_pipeline.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 20:15:16 by etran             #+#    #+#             */
/*   Updated: 2024/01/27 10:36:56 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

#include "compute_pipeline.h"

namespace scop {
class InputHandler;
}

namespace scop::gfx {

class TextureHandler;

/**
 * @brief Computes chunk culling, occlusion culling and frustum culling.
*/
class CullingPipeline: public ComputePipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = ComputePipeline;
	using super::DescriptorSetPtr;
	using TextureHandlerPtr = std::shared_ptr<TextureHandler>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	CullingPipeline();
	~CullingPipeline() = default;

	CullingPipeline(CullingPipeline&& other) = delete;
	CullingPipeline(const CullingPipeline& other) = delete;
	CullingPipeline& operator=(CullingPipeline&& other) = delete;
	CullingPipeline& operator=(const CullingPipeline& other) = delete;

	/* ========================================================================= */

	void				init(scop::core::Device& device) override;
	void				assemble(
		scop::core::Device& device,
		VkComputePipelineCreateInfo& info) override;
	void				plugDescriptor(
		scop::core::Device& device,
		const scop::InputHandler& input);

	void				compute(
		scop::core::Device& device,
		VkPipelineLayout layout,
		CommandBuffer& command_buffer) override;

	/* ========================================================================= */

	using super::getPipeline;
	using super::getDescriptor;

}; // class CullingPipeline

} // namespace scop::gfx