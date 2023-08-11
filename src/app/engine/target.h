/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 03:21:25 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:14:49 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif
# include <GLFW/glfw3.h>

# include <vector> // std::vector

# include "image_buffer.h"

namespace scop {
class Window;
}

namespace scop::graphics {

/**
 * @brief Wrapper class for target framebuffers.
*/
class Target {
public:
	/* ========================================================================= */
	/*                                HELPER CLASS                               */
	/* ========================================================================= */

	struct TargetInfo {
		using AttachmentVector = std::vector<VkImageView>;

		// per fb attachment vector
		VkRenderPass					render_pass;
		std::vector<AttachmentVector>	fb_attachments;
		uint32_t						width;
		uint32_t						height;
	};

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~Target() = default;

	/* ========================================================================= */

	virtual void				init(
		Device& device,
		const TargetInfo& info) = 0;
	virtual void				destroy(Device& device) = 0;
	virtual void				update(
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