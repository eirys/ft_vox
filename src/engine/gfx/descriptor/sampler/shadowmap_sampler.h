/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadowmap_sampler.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:12:27 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 16:26:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture_sampler.h"

namespace vox::gfx {

class ShadowmapSampler final: public TextureSampler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    ShadowmapSampler() = default;
    ~ShadowmapSampler() = default;

    ShadowmapSampler(ShadowmapSampler&& other) = delete;
    ShadowmapSampler(const ShadowmapSampler& other) = delete;
    ShadowmapSampler& operator=(ShadowmapSampler&& other) = delete;
    ShadowmapSampler& operator=(const ShadowmapSampler& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

private:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createSampler(const Device& device);

}; // class ShadowmapSampler

} // namespace vox::gfx