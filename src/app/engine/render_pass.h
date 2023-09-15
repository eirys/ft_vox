/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:50 by etran             #+#    #+#             */
/*   Updated: 2023/09/15 15:59:17 by etran            ###   ########.fr       */
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
class ImageBuffer;

class RenderPass {
public:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	/**
	 * @brief Render pass creation info.
	*/
	struct RenderPassInfo {
		uint32_t width;
		uint32_t height;
		VkFormat depth_format;
		VkSampleCountFlagBits depth_samples;
		VkFormat color_format;
		VkSampleCountFlagBits color_samples;

		const ImageBuffer*	texture_buffer;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual	~RenderPass() = default;

	/* ========================================================================= */

	virtual void	init(
		Device& device,
		const RenderPassInfo& rp_info) = 0;
	virtual void	destroy(Device& device);
	virtual void	updateResources(
		Device& device,
		const RenderPassInfo& rp_info) = 0;

	/* ========================================================================= */

	VkRenderPass	getRenderPass() const noexcept;
	uint32_t		getWidth() const noexcept;
	uint32_t		getHeight() const noexcept;

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
	RenderPass(RenderPass&& other) = default;
	RenderPass& operator=(RenderPass&& other) = default;

	RenderPass(const RenderPass& other) = delete;
	RenderPass& operator=(const RenderPass& other) = delete;

	/* ========================================================================= */

	virtual void	_createRenderPass(
		Device& device,
		const RenderPassInfo& create_info) = 0;

}; // class RenderPass

} // namespace scop::graphics