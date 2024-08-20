/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_texture.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:26:15 by etran             #+#    #+#             */
/*   Updated: 2024/07/12 18:31:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture.h"

namespace vox::gfx {

class PerlinNoiseTexture final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    PerlinNoiseTexture(): super(true) {}

    ~PerlinNoiseTexture() = default;

    PerlinNoiseTexture(PerlinNoiseTexture&& other) = delete;
    PerlinNoiseTexture(const PerlinNoiseTexture& other) = delete;
    PerlinNoiseTexture& operator=(PerlinNoiseTexture&& other) = delete;
    PerlinNoiseTexture& operator=(const PerlinNoiseTexture& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer) override;

}; // class PerlinNoiseTexture

} // namespace vox::gfx
