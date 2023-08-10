/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 03:12:47 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:12:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif
# include <GLFW/glfw3.h>

// Std
# include <memory> // std::shared_ptr

namespace scop::graphics {

class Device;

class Pipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using RenderPassPtr = std::shared_ptr<RenderPass>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~Pipeline() = default;

	/* ========================================================================= */

	virtual void		init(
		Device& device,
		VkGraphicsPipelineCreateInfo& layout_info) = 0;
	virtual void		destroy(Device& device) = 0;
	virtual void		record(
		Device& device,
		VkPipelineLayout layout,
		VkCommandBuffer command_buffer) = 0;


	/* ========================================================================= */

	VkPipeline				getPipeline() const noexcept;
	virtual RenderPassPtr	getRenderPass() const noexcept = 0;

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkPipeline			_pipeline;

	RenderPassPtr		_render_pass;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Pipeline() = default;

	Pipeline(Pipeline&& other) = delete;
	Pipeline(const Pipeline& other) = delete;
	Pipeline& operator=(Pipeline&& other) = delete;
	Pipeline& operator=(const Pipeline& other) = delete;

	/* ========================================================================= */

	virtual void		_createPipeline(
		Device& device,
		VkGraphicsPipelineCreateInfo& info) = 0;
	virtual void		_populateShaderStages(
		Device& device,
		std::vector<VkPipelineShaderStageCreateInfo>& stages) = 0;

	VkShaderModule		_createShaderModule(
		Device& device,
		const std::string& path);

}; // class Pipeline

} // namespace scop::graphics