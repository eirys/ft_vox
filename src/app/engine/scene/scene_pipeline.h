/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:31:39 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:34:41 by etran            ###   ########.fr       */
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
		CommandPool& pool,
		const RenderPass::RenderPassInfo& rp_info,
		const std::vector<Texture>& textures,
		VkGraphicsPipelineCreateInfo& info) override;
	void	record(
		Device& device,
		VkPipelineLayout layout,
		VkCommandBuffer command_buffer,
		InputHandler& input) override;

	/* ========================================================================= */

	using super::getPipeline;
	using super::getRenderPass;
	using super::getTextureHandler;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void	_createRenderPass(
		Device& device,
		const RenderPass::RenderPassInfo& rp_info) override;
	void	_createTextureHandler(
		Device& device,
		CommandPool& pool,
		const std::vector<Texture>& textures) override;
	void	_createPipeline(
		Device& device,
		VkGraphicsPipelineCreateInfo& info) override;

}; // class ScenePipeline

} // namespace scop::graphics