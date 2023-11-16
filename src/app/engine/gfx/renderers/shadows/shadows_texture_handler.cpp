/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_texture_handler.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 16:50:17 by etran             #+#    #+#             */
/*   Updated: 2023/11/16 23:34:20 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shadows_texture_handler.h"
#include "device.h"

#include "shadowmap_consts.h"

#include <stdexcept> // std::invalid_argument

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	ShadowsTextureHandler::init(scop::core::Device& device) {
	ImageMetaData	data{};
	data.format = SHADOWMAP_DEPTH_FORMAT;
	data.layer_count = 1;
	data.width = SHADOWMAP_SIZE;
	data.height = SHADOWMAP_SIZE;

	super::_texture_buffer.setMetaData(data);

	_createTextureImages(device);
	_createTextureImageView(device);
	_createTextureSampler(device);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void	ShadowsTextureHandler::_createTextureImages(scop::core::Device& device) {
	super::_texture_buffer.initImage(
		device,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
		VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_SAMPLE_COUNT_1_BIT);
}

void	ShadowsTextureHandler::_createTextureImageView(scop::core::Device& device) {
	super::_texture_buffer.initView(
		device,
		VK_IMAGE_ASPECT_DEPTH_BIT);
}

void	ShadowsTextureHandler::_createTextureSampler(scop::core::Device& device) {
	VkSamplerCreateInfo	sampler{};
	sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler.magFilter = VK_FILTER_LINEAR;
	sampler.minFilter = VK_FILTER_LINEAR;
	sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler.anisotropyEnable = VK_FALSE;
	sampler.maxAnisotropy = 1.0;
	sampler.compareEnable = VK_FALSE;
	sampler.compareOp = VK_COMPARE_OP_ALWAYS;
	sampler.mipLodBias = 0.0f;
	sampler.minLod = 0.0f;
	sampler.maxLod = 1.0f;
	sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

	if (vkCreateSampler(device.getLogicalDevice(), &sampler, nullptr, &(super::_texture_sampler)) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler");
	}
}

} // namespace scop::gfx