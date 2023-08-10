/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:31:39 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:10:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <memory> // std::shared_ptr

# include "pipeline.h"
# include "scene_render_pass.h"

namespace scop::graphics {

class Device;

class ScenePipeline final: public Pipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using SceneRenderPassPtr = std::shared_ptr<SceneRenderPass>;

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

	void	init(
		Device& device,
		VkGraphicsPipelineCreateInfo& info) override;
	void	destroy(Device& device) override;
	void	record(
		Device& device,
		VkCommandBuffer command_buffer) override;

	/* ========================================================================= */

	using Pipeline::getPipeline;

	RenderPassPtr	getRenderPass() const noexcept override;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	SceneRenderPassPtr		_render_pass;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	using Pipeline::_createShaderModule;

	void					_createPipeline(
		Device& device,
		VkGraphicsPipelineCreateInfo& info) override;
	void					_populateShaderStages(
		Device& device,
		std::vector<VkPipelineShaderStageCreateInfo>& stages) override;

}; // class ScenePipeline

} // namespace scop::graphics