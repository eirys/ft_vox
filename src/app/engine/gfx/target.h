/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 03:21:25 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:11:49 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

# include <vector> // std::vector
# include <memory>

# include "image_buffer.h"

namespace scop {
class Window;
class SwapChain;
}

namespace scop::core {
class Device;
}

namespace scop::gfx {

class RenderPass;

/* ========================================================================= */
/*                               HELPER OBJECTS                              */
/* ========================================================================= */

/**
 * @brief Target creation info.
*/
struct TargetInfo {
	scop::SwapChain*		swap_chain;
	std::shared_ptr<RenderPass>	render_pass;
};

/**
 * @brief Wrapper class for target framebuffers.
*/
class Target {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~Target() = default;

	/* ========================================================================= */

	virtual void		init(
		scop::core::Device& device,
		const TargetInfo& info) = 0;
	void				destroy(scop::core::Device& device);
	virtual void		update(
		scop::core::Device& device,
		const TargetInfo& info) = 0;

	/* ========================================================================= */

	const std::vector<VkFramebuffer>&	getFrameBuffers() const noexcept;

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	std::vector<VkFramebuffer>	_frame_buffers;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Target() = default;

	Target(Target&& other) = delete;
	Target(const Target& other) = delete;
	Target& operator=(Target&& other) = delete;
	Target& operator=(const Target& other) = delete;

}; // class Target

} // namespace scop::gfx