/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampler.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:10:03 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 15:58:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "image_buffer.h"

namespace vox::gfx {

class TextureSampler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~TextureSampler() = default;

    /* ====================================================================== */

    virtual void init(const Device& device) = 0;
    virtual void destroy(const Device& device) = 0;

    virtual void fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) = 0;

    /* ====================================================================== */

    const ImageBuffer&  getImageBuffer() const noexcept { return m_imageBuffer; }
    VkSampler           getSampler() const noexcept { return m_sampler; }

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    ImageBuffer m_imageBuffer;
    VkSampler   m_sampler;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    TextureSampler() = default;

    TextureSampler(TextureSampler&& other) = delete;
    TextureSampler(const TextureSampler& other) = delete;
    TextureSampler& operator=(TextureSampler&& other) = delete;
    TextureSampler& operator=(const TextureSampler& other) = delete;

}; // class TextureSampler

} // namespace vox::gfx
