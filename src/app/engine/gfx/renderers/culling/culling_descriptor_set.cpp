/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_descriptor_set.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 16:54:52 by etran             #+#    #+#             */
/*   Updated: 2023/11/19 19:37:06 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "culling_descriptor_set.h"
#include "device.h"

namespace scop::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void CullingDescriptorSet::init(scop::core::Device& device) {
	VkDescriptorSetLayoutBinding	height{};
	height.binding = 0;
	height.descriptorCount = 1;
	height.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	height.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	++super::_writes_sizes.storage_image;

	VkDescriptorSetLayoutBinding	culling{};
	culling.binding = 1;
	culling.descriptorCount = 1;
	culling.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	culling.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	++super::_writes_sizes.storage_image;

}

} // namespace scop::gfx


} // namespace scop::gfx