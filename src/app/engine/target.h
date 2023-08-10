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

namespace scop::graphics {

class Target {
public:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Target() = default;
	~Target() = default;

	Target(Target&& other) = delete;
	Target(const Target& other) = delete;
	Target& operator=(Target&& other) = default;
	Target& operator=(const Target& other) = delete;

	/* ========================================================================= */

	void						init(
		Device& device,
		::scop::Window& window
	);
	void						destroy(Device& device);

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	std::vector<VkImage>		_images;
	std::vector<VkImageView>	_image_views;
	std::vector<VkFramebuffer>	_frame_buffers;

}; // class Target

} // namespace scop::graphics