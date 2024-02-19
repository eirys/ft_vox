/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:31:39 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 12:55:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <memory> // std::shared_ptr

# include "graphics_pipeline.h"
# include "buffer.h"

namespace scop::gfx {

class ScenePipeline final: public GraphicsPipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = GraphicsPipeline;
	using super::DescriptorSetPtr;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ScenePipeline();
	~ScenePipeline() = default;

	ScenePipeline(ScenePipeline&& other) = delete;
	ScenePipeline(const ScenePipeline& other) = delete;
	ScenePipeline& operator=(ScenePipeline&& other) = delete;
	ScenePipeline& operator=(const ScenePipeline& other) = delete;

	/* ========================================================================= */

	void	init(
		scop::core::Device& device,
		RenderPassInfo& rp_info,
		TargetInfo& tar_info) override;
	void	assemble(
		scop::core::Device& device,
		VkGraphicsPipelineCreateInfo& info) override;
	void	plugDescriptor(
		scop::core::Device& device,
		TextureHandlerPtr shadowmap,
		const scop::InputHandler& input);
	void	destroy(scop::core::Device& device) override;

	void	draw(
		VkPipelineLayout layout,
		CommandBuffer& command_buffer,
		const scop::InputHandler& input,
		int32_t image_index) override;
	void	update(const scop::UniformBufferObject& ubo) noexcept override;

	/* ========================================================================= */

	using super::getPipeline;
	using super::getRenderPass;
	using super::getTextureHandler;
	using super::getTarget;
	using super::getDescriptor;

	Buffer&	getUbo() noexcept;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void	_beginRenderPass(
		CommandBuffer& command_buffer,
		int32_t image_index = 0) override;

	/* ========================================================================= */
	/*                                    DATA                                   */
	/* ========================================================================= */

	Buffer	_ubo;

}; // class ScenePipeline

} // namespace scop::gfx