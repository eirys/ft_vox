/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_pipeline.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 21:11:38 by etran             #+#    #+#             */
/*   Updated: 2023/11/19 11:45:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "compute_pipeline.h"

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

VkPipeline ComputePipeline::getPipeline() const noexcept {
	return _pipeline;
}

ComputePipeline::DescriptorSetPtr ComputePipeline::getDescriptor() const noexcept {
	return _descriptor;
}

} // namespace scop::gfx