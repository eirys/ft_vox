/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_manager.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 21:59:24 by etran             #+#    #+#             */
/*   Updated: 2023/12/05 17:16:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
#include <memory>

namespace scop {
class SwapChain;
class InputHandler;
}
namespace scop::core {
class Device;
}
namespace scop::gfx {

class GraphicsPipeline;
class ComputePipeline;

/**
 * @brief Manager for all pipelines.
*/
class PipelineManager {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using GraphicsPipelinePtr = std::shared_ptr<GraphicsPipeline>;
	using ComputePipelinePtr = std::shared_ptr<ComputePipeline>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	PipelineManager();

	~PipelineManager() = default;

	PipelineManager(PipelineManager&& other) = delete;
	PipelineManager(const PipelineManager& other) = delete;
	PipelineManager& operator=(PipelineManager&& other) = delete;
	PipelineManager& operator=(const PipelineManager& other) = delete;

	/* ========================================================================= */

	void	init(scop::core::Device& device, const scop::SwapChain& swap_chain);
	void	destroy(scop::core::Device& device);

	void	plugDescriptors(scop::core::Device& device, InputHandler& input_handler);

	/* ========================================================================= */

	VkPipelineLayout		getPipelineLayout() const noexcept;

	GraphicsPipelinePtr		getScenePipeline() const noexcept;
	GraphicsPipelinePtr		getShadowsPipeline() const noexcept;
	ComputePipelinePtr		getCullingPipeline() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkPipelineLayout		_pipeline_layout;

	struct {
		GraphicsPipelinePtr	scene;
		GraphicsPipelinePtr	shadows;
	}						_graphics;

	struct {
		ComputePipelinePtr	culling;
	}						_compute;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void					_createComputesPipelines(scop::core::Device& device);
	void					_createGraphicsPipelines(
		scop::core::Device& device,
		const scop::SwapChain& swap_chain);

	void					_createPipelineLayout(scop::core::Device& device);

	void					_assembleComputePipelines(scop::core::Device& device);
	void					_assembleGraphicsPipelines(scop::core::Device& device);

	void					_createDescriptor(scop::core::Device& device);

}; // class PipelineManager

} // namespace scop