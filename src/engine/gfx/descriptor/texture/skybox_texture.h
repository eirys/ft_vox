/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_texture.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:48:23 by etran             #+#    #+#             */
/*   Updated: 2024/06/20 16:27:41 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture.h"

namespace vox::gfx {

class SkyboxSampler final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    SkyboxSampler(): super(true) {}

    ~SkyboxSampler() = default;

    SkyboxSampler(SkyboxSampler&& other) = delete;
    SkyboxSampler(const SkyboxSampler& other) = delete;
    SkyboxSampler& operator=(SkyboxSampler&& other) = delete;
    SkyboxSampler& operator=(const SkyboxSampler& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

}; // class SkyboxSampler

} // namespace vox::gfx