/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_texture.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:26:15 by etran             #+#    #+#             */
/*   Updated: 2024/06/20 16:11:04 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture.h"

namespace vox::gfx {

class PerlinNoiseSampler final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    PerlinNoiseSampler(): super(true) {}

    ~PerlinNoiseSampler() = default;

    PerlinNoiseSampler(PerlinNoiseSampler&& other) = delete;
    PerlinNoiseSampler(const PerlinNoiseSampler& other) = delete;
    PerlinNoiseSampler& operator=(PerlinNoiseSampler&& other) = delete;
    PerlinNoiseSampler& operator=(const PerlinNoiseSampler& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

}; // class PerlinNoiseSampler

} // namespace vox::gfx
