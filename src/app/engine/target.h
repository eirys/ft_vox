/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 03:21:25 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 22:54:27 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif
# include <GLFW/glfw3.h>

# include <vector> // std::vector
# include <memory>

# include "image_buffer.h"

namespace scop {
class Window;
}

namespace scop::graphics {

class Device;
class RenderPass;

/**
 * @brief Wrapper class for target framebuffers.
*/
class Target {
public:
	/* ========================================================================= */
	/*                                HELPER CLASS                               */
	/* ========================================================================= */

	/**
	 * @brief Target creation info.
	*/
	struct TargetInfo {
		std::vector<VkImageView>		swap_views;
		uint32_t						width;
		uint32_t						height;
		std::shared_ptr<RenderPass>		render_pass;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~Target() = default;

	/* ========================================================================= */

	virtual void		init(
		Device& device,
		const TargetInfo& info) = 0;
	void				destroy(Device& device);
	virtual void		update(
		Device& device,
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

} // namespace scop::graphics