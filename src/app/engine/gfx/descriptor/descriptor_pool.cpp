/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_pool.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:00:20 by etran             #+#    #+#             */
/*   Updated: 2024/02/28 16:16:37 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_pool.h"
#include "device.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void DescriptorPool::init(const Device& device) {
    _createPool(device);
    _allocatePool(device);
}

void DescriptorPool::destroy(const Device& device) {
    vkDestroyDescriptorPool(device.get(), m_pool, nullptr);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void DescriptorPool::_createPool(const Device& device, const DescriptorTable& table) {
}

} // namespace vox::gfx
