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

# include "render_pass.h"
# include "texture_handler.h"
# include "target.h"

// Std
# include <memory> // std::shared_ptr
# include <string> // std::string

namespace scop {
class Image;
}

namespace scop::graphics {

class Device;
class InputHandler;

class Pipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using RenderPassPtr = std::shared_ptr<RenderPass>;
	using TextureHandlerPtr = std::shared_ptr<TextureHandler>;
	using TargetPtr = std::shared_ptr<Target>;
	using Texture = ::scop::Image;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~Pipeline() = default;

	/* ========================================================================= */

	virtual void		init(
		Device& device,
		CommandPool& pool,
		const RenderPass::RenderPassInfo& rp_info,
		const std::vector<Texture>& textures,
		VkGraphicsPipelineCreateInfo& layout_info) = 0;
	void				destroy(Device& device);
	virtual void		record(
		Device& device,
		VkPipelineLayout layout,
		VkCommandBuffer command_buffer,
		InputHandler& input) = 0;


	/* ========================================================================= */

	VkPipeline			getPipeline() const noexcept;
	RenderPassPtr		getRenderPass() const noexcept;
	TextureHandlerPtr	getTextureHandler() const noexcept;

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkPipeline			_pipeline;

	RenderPassPtr		_render_pass;
	TextureHandlerPtr	_texture_handler;
	TargetPtr			_target;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Pipeline() = default;

	Pipeline(Pipeline&& other) = delete;
	Pipeline(const Pipeline& other) = delete;
	Pipeline& operator=(Pipeline&& other) = delete;
	Pipeline& operator=(const Pipeline& other) = delete;

	/* ========================================================================= */

	virtual void		_createRenderPass(
		Device& device,
		const RenderPass::RenderPassInfo& rp_info) = 0;
	virtual void		_createTextureHandler(
		Device& device,
		CommandPool& pool,
		const std::vector<Texture>& textures) = 0;
	virtual void		_createPipeline(
		Device& device,
		VkGraphicsPipelineCreateInfo& info) = 0;

	/* UTILS =================================================================== */

	VkShaderModule		_createShaderModule(
		Device& device,
		const std::string& path);

}; // class Pipeline

} // namespace scop::graphics