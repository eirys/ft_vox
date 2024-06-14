/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gbuffer_textures.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 00:06:31 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 02:19:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture_sampler.h"

namespace vox::gfx {

class PositionTexture final: public TextureSampler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    PositionTexture() = default;
    ~PositionTexture() = default;

    PositionTexture(PositionTexture&& other) = delete;
    PositionTexture(const PositionTexture& other) = delete;
    PositionTexture& operator=(PositionTexture&& other) = delete;
    PositionTexture& operator=(const PositionTexture& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

}; // class PositionTexture

class NormalTexture final: public TextureSampler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    NormalTexture() = default;
    ~NormalTexture() = default;

    NormalTexture(NormalTexture&& other) = delete;
    NormalTexture(const NormalTexture& other) = delete;
    NormalTexture& operator=(NormalTexture&& other) = delete;
    NormalTexture& operator=(const NormalTexture& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

}; // class NormalTexture

class AlbedoTexture final: public TextureSampler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    AlbedoTexture() = default;
    ~AlbedoTexture() = default;

    AlbedoTexture(AlbedoTexture&& other) = delete;
    AlbedoTexture(const AlbedoTexture& other) = delete;
    AlbedoTexture& operator=(AlbedoTexture&& other) = delete;
    AlbedoTexture& operator=(const AlbedoTexture& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

}; // class AlbedoTexture

} // namespace vox::gfx