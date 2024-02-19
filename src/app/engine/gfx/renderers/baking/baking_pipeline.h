/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baking_pipeline.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:34:15 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 12:55:47 by etran            ###   ########.fr       */
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
 * @brief Bakes the scene visible faces of the each block.
*/
class BakingPipeline: public ComputePipeline {
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

	BakingPipeline();
	~BakingPipeline() = default;

	BakingPipeline(BakingPipeline&& other) = delete;
	BakingPipeline(const BakingPipeline& other) = delete;
	BakingPipeline& operator=(BakingPipeline&& other) = delete;
	BakingPipeline& operator=(const BakingPipeline& other) = delete;

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

	TextureHandlerPtr	getCullingTextureHandler() const;

private:
	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	TextureHandlerPtr	_chunk_texture;

}; // class BakingPipeline

} // namespace scop::gfx