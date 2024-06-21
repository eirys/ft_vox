/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sampler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:01:18 by etran             #+#    #+#             */
/*   Updated: 2024/06/20 16:39:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sampler.h"
#include "device.h"

#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void Sampler::init(const Device& device, const Sampler::Metadata& metadata) {
    m_metadata = metadata;

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = (VkFilter)m_metadata.m_filter;
    samplerInfo.minFilter = (VkFilter)m_metadata.m_filter;
    samplerInfo.mipmapMode = (VkSamplerMipmapMode)m_metadata.m_mipMode;
    samplerInfo.addressModeU = (VkSamplerAddressMode)m_metadata.m_border;
    samplerInfo.addressModeV = (VkSamplerAddressMode)m_metadata.m_border;
    samplerInfo.addressModeW = (VkSamplerAddressMode)m_metadata.m_border;
    samplerInfo.borderColor = (VkBorderColor)m_metadata.m_border;
    samplerInfo.maxLod = (float)m_metadata.m_maxLod;
    samplerInfo.minLod = 0.0f;
    samplerInfo.mipLodBias = 0.0f;

    if (m_metadata.m_enableAnisotropy) {
        auto properties = device.queryDeviceProperties();
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    } else {
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
    }

    if (m_metadata.m_compare == Sampler::Compare::Never) {
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    } else {
        samplerInfo.compareEnable = VK_TRUE;
        samplerInfo.compareOp = (VkCompareOp)m_metadata.m_compare;
    }

    if (vkCreateSampler(device.getDevice(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void Sampler::destroy(const Device& device) {
    vkDestroySampler(device.getDevice(), m_sampler, nullptr);
}

/* ========================================================================== */

VkSampler Sampler::getSampler() const noexcept {
    return m_sampler;
}

const Sampler::Metadata& Sampler::getMetadata() const noexcept {
    return m_metadata;
}

} // namespace vox::gfx