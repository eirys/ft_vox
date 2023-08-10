/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pass.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:50 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:26:29 by etran            ###   ########.fr       */
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
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual	~RenderPass() = default;

	/* ========================================================================= */

	virtual void	init(Device& device, SwapChain& swap_chain) = 0;
	void			destroy(Device& device);

	/* ========================================================================= */

	VkRenderPass	getRenderPass() const noexcept;

protected:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	RenderPass() = default;

	RenderPass(RenderPass&& other) = delete;
	RenderPass(const RenderPass& other) = delete;
	RenderPass& operator=(RenderPass&& other) = delete;
	RenderPass& operator=(const RenderPass& other) = delete;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	uint32_t		_width;
	uint32_t		_height;

	VkRenderPass	_render_pass;

}; // class RenderPass

} // namespace scop::graphics