/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:10:03 by etran             #+#    #+#             */
/*   Updated: 2024/06/25 16:30:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "image_buffer.h"

namespace vox::gfx {

class Texture {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    virtual ~Texture() = default;

    /* ====================================================================== */

    virtual void init(const Device& device) = 0;
    virtual void destroy(const Device& device) = 0;

    virtual void fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) = 0;

    /* ====================================================================== */

    const ImageBuffer&  getImageBuffer() const noexcept { return m_imageBuffer; }

protected:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    ImageBuffer m_imageBuffer;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Texture(bool isSampled): m_imageBuffer(isSampled) {}

    Texture() = delete;
    Texture(Texture&& other) = delete;
    Texture(const Texture& other) = delete;
    Texture& operator=(Texture&& other) = delete;
    Texture& operator=(const Texture& other) = delete;

}; // class Texture

} // namespace vox::gfx
