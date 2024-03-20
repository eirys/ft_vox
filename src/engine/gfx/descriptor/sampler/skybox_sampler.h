/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_sampler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:48:23 by etran             #+#    #+#             */
/*   Updated: 2024/03/20 15:27:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture_sampler.h"

namespace vox::gfx {

class SkyboxSampler final: public TextureSampler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    SkyboxSampler() = default;
    ~SkyboxSampler() = default;

    SkyboxSampler(SkyboxSampler&& other) = delete;
    SkyboxSampler(const SkyboxSampler& other) = delete;
    SkyboxSampler& operator=(SkyboxSampler&& other) = delete;
    SkyboxSampler& operator=(const SkyboxSampler& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

private:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createSampler(const Device& device);

}; // class SkyboxSampler

} // namespace vox::gfx