/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_pipeline.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:59:15 by etran             #+#    #+#             */
/*   Updated: 2023/12/31 17:51:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics_pipeline.h"
#include "device.h"

#include "render_pass.h"
#include "texture_handler.h"
#include "target.h"
#include "descriptor_set.h"

#include <stdexcept> // std::runtime_error

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	GraphicsPipeline::destroy(scop::core::Device& device) {
	_descriptor->destroy(device);
	_render_pass->destroy(device);
	_target->destroy(device);
	_texture->destroy(device);
	vkDestroyPipeline(device.getLogicalDevice(), _pipeline, nullptr);
}

void	GraphicsPipeline::update(const UniformBufferObject& ubo) noexcept {
	(void)ubo;
}

/* ========================================================================== */

VkPipeline	GraphicsPipeline::getPipeline() const noexcept {
	return _pipeline;
}

GraphicsPipeline::RenderPassPtr	GraphicsPipeline::getRenderPass() const noexcept {
	return _render_pass;
}

GraphicsPipeline::TextureHandlerPtr	GraphicsPipeline::getTextureHandler() const noexcept {
	return _texture;
}

GraphicsPipeline::TargetPtr	GraphicsPipeline::getTarget() const noexcept {
	return _target;
}

GraphicsPipeline::DescriptorSetPtr	GraphicsPipeline::getDescriptor() const noexcept {
	return _descriptor;
}

} // namespace scop::gfx