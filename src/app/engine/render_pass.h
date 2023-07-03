/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:50 by etran             #+#    #+#             */
/*   Updated: 2023/07/03 10:15:16 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

namespace scop {
namespace graphics {
class Device;
class SwapChain;

class RenderPass {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	RenderPass() = default;
	~RenderPass() = default;

	RenderPass(RenderPass&& other) = delete;
	RenderPass(const RenderPass& other) = delete;
	RenderPass& operator=(RenderPass&& other) = delete;
	RenderPass& operator=(const RenderPass& other) = delete;

	/* ========================================================================= */

	void			init(Device& device, SwapChain& swap_chain);
	void			destroy(Device& device);

	/* ========================================================================= */

	VkRenderPass	getRenderPass() const noexcept;

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkRenderPass	_render_pass;

}; // class RenderPass

} // namespace graphics
} // namespace scop

