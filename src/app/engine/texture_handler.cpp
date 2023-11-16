/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 20:25:44 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:33:43 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture_handler.h"
#include "device.h"

#include <cmath> // std::floor

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	TextureHandler::destroy(scop::core::Device& device) {
	vkDestroySampler(device.getLogicalDevice(), _texture_sampler, nullptr);
	_texture_buffer.destroy(device);
}

/* ========================================================================== */

VkSampler	TextureHandler::getTextureSampler() const noexcept {
	return _texture_sampler;
}

const scop::core::ImageBuffer&	TextureHandler::getTextureBuffer() const noexcept {
	return _texture_buffer;
}

/* ========================================================================== */

void	TextureHandler::setTextureSampler(VkSampler sampler) {
	_texture_sampler = sampler;
}

/* ========================================================================== */
/*                                  PROTECTED                                 */
/* ========================================================================== */

uint32_t	TextureHandler::_getMipLevelCount(uint32_t image_width) const {
	return 1 + static_cast<uint32_t>(std::floor(std::log2(image_width)));
}

} // namespace scop::gfx