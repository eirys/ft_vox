/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:31:39 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:05:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <memory> // std::shared_ptr

# include "pipeline.h"
# include "buffer.h"

namespace scop::gfx {

class ScenePipeline final: public Pipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = Pipeline;
	using super::Texture;
	using super::DescriptorSetPtr;

	using super::UniformBufferObject;
	using super::Camera;

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
	void	update(const UniformBufferObject& ubo) noexcept override;

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
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	Buffer	_ubo;

}; // class ScenePipeline

} // namespace scop::gfx