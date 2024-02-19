/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:50 by etran             #+#    #+#             */
/*   Updated: 2024/01/20 12:55:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

namespace scop::core {
class Device;
}

namespace scop::gfx {

class ImageBuffer;

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

class RenderPass {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual	~RenderPass() = default;

	/* ========================================================================= */

	virtual void	init(
		scop::core::Device& device,
		const RenderPassInfo& rp_info) = 0;
	virtual void	destroy(scop::core::Device& device);
	virtual void	updateResources(
		scop::core::Device& device,
		const RenderPassInfo& rp_info) = 0;

	/* ========================================================================= */

	VkRenderPass	getRenderPass() const noexcept;
	uint32_t		getWidth() const noexcept;
	uint32_t		getHeight() const noexcept;

protected:
	/* ========================================================================= */
	/*                                    DATA                                   */
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
		scop::core::Device& device,
		const RenderPassInfo& create_info) = 0;

}; // class RenderPass

} // namespace scop::gfx