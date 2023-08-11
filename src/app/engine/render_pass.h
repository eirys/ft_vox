/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:50 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 22:53:55 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

namespace scop::graphics {

class Device;
class Pipeline;
class SwapChain;

class RenderPass {
public:
	/* ========================================================================= */
	/*                                HELPER CLASS                               */
	/* ========================================================================= */

	/**
	 * @brief Render pass creation info.
	*/
	struct RenderPassInfo {
		uint32_t width;
		uint32_t height;
		VkFormat color_format;
		VkSampleCountFlagBits color_samples;
		VkFormat depth_format;
		VkSampleCountFlagBits depth_samples;
	};

	/**
	 * @brief Render pass resources creation info.
	*/
	struct ResourcesInfo {
		uint32_t width;
		uint32_t height;
		VkFormat color_format;
		VkFormat depth_format;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual	~RenderPass() = default;

	/* ========================================================================= */

	virtual void	init(
		Device& device,
		const RenderPassInfo& rp_info,
		const ResourcesInfo& res_info) = 0;
	virtual void	destroy(Device& device);
	virtual void	updateResources(
		Device& device,
		const ResourcesInfo& res_info) = 0;

	/* ========================================================================= */

	VkRenderPass	getRenderPass() const noexcept;

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint32_t		_width;
	uint32_t		_height;

	VkRenderPass	_render_pass;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	RenderPass() = default;

	RenderPass(RenderPass&& other) = delete;
	RenderPass(const RenderPass& other) = delete;
	RenderPass& operator=(RenderPass&& other) = delete;
	RenderPass& operator=(const RenderPass& other) = delete;

	/* ========================================================================= */

	virtual void	_createRenderPass(
		Device& device,
		const RenderPassInfo& create_info) = 0;
	virtual void	_createResources(
		Device& device,
		const ResourcesInfo& res_info) = 0;
	virtual void	_destroyResources(Device& device) = 0;

}; // class RenderPass

} // namespace scop::graphics