/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_pipeline.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 21:11:38 by etran             #+#    #+#             */
/*   Updated: 2024/01/08 14:55:14 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "compute_pipeline.h"
#include "device.h"
#include "descriptor_set.h"

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void	ComputePipeline::destroy(scop::core::Device& device) {
	_descriptor->destroy(device);
	vkDestroyPipeline(device.getLogicalDevice(), _pipeline, nullptr);
}

VkPipeline ComputePipeline::getPipeline() const noexcept {
	return _pipeline;
}

ComputePipeline::DescriptorSetPtr ComputePipeline::getDescriptor() const noexcept {
	return _descriptor;
}

} // namespace scop::gfx