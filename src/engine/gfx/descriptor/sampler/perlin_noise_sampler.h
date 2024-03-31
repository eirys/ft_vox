/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_sampler.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:26:15 by etran             #+#    #+#             */
/*   Updated: 2024/03/27 15:43:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture_sampler.h"

namespace vox::gfx {

class PerlinNoiseSampler final: public TextureSampler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    PerlinNoiseSampler() = default;
    ~PerlinNoiseSampler() = default;

    PerlinNoiseSampler(PerlinNoiseSampler&& other) = delete;
    PerlinNoiseSampler(const PerlinNoiseSampler& other) = delete;
    PerlinNoiseSampler& operator=(PerlinNoiseSampler&& other) = delete;
    PerlinNoiseSampler& operator=(const PerlinNoiseSampler& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

private:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createSampler(const Device& device);

}; // class PerlinNoiseSampler

} // namespace vox::gfx
