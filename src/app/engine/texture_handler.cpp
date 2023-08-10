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
#include "engine.h"
#include "image_handler.h"
#include "device.h"
#include "utils.h"
#include "buffer.h"
#include "command_pool.h"
#include "command_buffer.h"

#include <cmath> // std::floor
#include <algorithm> // std::max
#include <stdexcept> // std::runtime_error
#include <cstring> // memcpy

namespace scop::graphics {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	TextureHandler::destroy(Device& device) {
	vkDestroySampler(device.getLogicalDevice(), _vk_texture_sampler, nullptr);
	_texture_buffer.destroy(device);
}

/* ========================================================================== */

VkSampler	TextureHandler::getTextureSampler() const noexcept {
	return _vk_texture_sampler;
}

const ImageBuffer&	TextureHandler::getTextureBuffer() const noexcept {
	return _texture_buffer;
}

} // namespace scop::graphics