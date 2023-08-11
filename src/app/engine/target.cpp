/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 03:26:34 by etran             #+#    #+#             */
/*   Updated: 2023/08/11 21:55:05 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "target.h"
#include "device.h"

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	Target::destroy(Device& device) {
	for (auto& frame_buffer: _frame_buffers) {
		vkDestroyFramebuffer(
			device.getLogicalDevice(),
			frame_buffer,
			nullptr);
	}
}

/* ========================================================================== */

const std::vector<VkFramebuffer>&	Target::getFrameBuffers() const noexcept {
	return _frame_buffers;
}

} // namespace scop::graphics