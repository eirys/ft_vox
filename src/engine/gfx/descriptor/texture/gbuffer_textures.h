/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gbuffer_textures.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 00:06:31 by etran             #+#    #+#             */
/*   Updated: 2024/06/25 14:57:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vox_decl.h"
#include "texture.h"

namespace vox::gfx {

class PositionTexture;
class NormalTexture;
class AlbedoTexture;
class DepthTexture;
class SSAOTexture;
using NormalViewTexture = NormalTexture;
using SSAOBlurTexture = SSAOTexture;

#if ENABLE_HIGH_RES
class PositionViewTexture;
#else
using PositionViewTexture = PositionTexture;
#endif

class PositionTexture final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    PositionTexture(): super(true) {}

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

class NormalTexture final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    NormalTexture(): super(true) {}

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

class AlbedoTexture final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    AlbedoTexture(): super(true) {}

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

class DepthTexture final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    DepthTexture(): super(true) {}

    ~DepthTexture() = default;

    DepthTexture(DepthTexture&& other) = delete;
    DepthTexture(const DepthTexture& other) = delete;
    DepthTexture& operator=(DepthTexture&& other) = delete;
    DepthTexture& operator=(const DepthTexture& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

}; // class DepthTexture

#if ENABLE_HIGH_RES
class PositionViewTexture final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    PositionViewTexture(): super(true) {}

    ~PositionViewTexture() = default;

    PositionViewTexture(PositionViewTexture&& other) = delete;
    PositionViewTexture(const PositionViewTexture& other) = delete;
    PositionViewTexture& operator=(PositionViewTexture&& other) = delete;
    PositionViewTexture& operator=(const PositionViewTexture& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

}; // class PositionViewTexture
#endif

/**
 * @brief SSAO texture sampler, same format used for the SSAO Blur texture.
 */
class SSAOTexture final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    SSAOTexture(): super(true) {}

    ~SSAOTexture() = default;

    SSAOTexture(SSAOTexture&& other) = delete;
    SSAOTexture(const SSAOTexture& other) = delete;
    SSAOTexture& operator=(SSAOTexture&& other) = delete;
    SSAOTexture& operator=(const SSAOTexture& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

}; // class SSAOTexture

} // namespace vox::gfx