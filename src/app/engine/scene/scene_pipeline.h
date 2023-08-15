/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:31:39 by etran             #+#    #+#             */
/*   Updated: 2023/08/15 19:21:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <memory> // std::shared_ptr

# include "pipeline.h"

namespace scop::graphics {

class Device;

class ScenePipeline final: public Pipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = Pipeline;
	using Texture = super::Texture;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ScenePipeline() = default;
	~ScenePipeline() = default;

	ScenePipeline(ScenePipeline&& other) = delete;
	ScenePipeline(const ScenePipeline& other) = delete;
	ScenePipeline& operator=(ScenePipeline&& other) = delete;
	ScenePipeline& operator=(const ScenePipeline& other) = delete;

	/* ========================================================================= */

	using super::destroy;

	void	init(
		Device& device,
		const RenderPass::RenderPassInfo& rp_info,
		const RenderPass::ResourcesInfo& res_info,
		Target::TargetInfo& tar_info,
		const std::vector<Texture>& textures,
		VkGraphicsPipelineCreateInfo& info) override;
	void	draw(
		Device& device,
		VkPipelineLayout layout,
		CommandBuffer& command_buffer,
		InputHandler& input,
		int32_t image_index) override;
	void	update(const ::scop::UniformBufferObject& ubo) noexcept override;

	/* ========================================================================= */

	using super::getPipeline;
	using super::getRenderPass;
	using super::getTextureHandler;
	using super::getTarget;
	using super::getDescriptor;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void	_createRenderPass(
		Device& device,
		const RenderPass::RenderPassInfo& rp_info,
		const RenderPass::ResourcesInfo& res_info) override;
	void	_createTarget(
		Device& device,
		Target::TargetInfo& info) override;
	void	_createTextureHandler(
		Device& device,
		const std::vector<Texture>& textures) override;
	void	_createDescriptor(
		Device& device);
	void	_createPipeline(
		Device& device,
		VkGraphicsPipelineCreateInfo& info) override;

}; // class ScenePipeline

} // namespace scop::graphics