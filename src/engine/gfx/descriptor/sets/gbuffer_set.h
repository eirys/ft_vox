/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gbuffer_set.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 23:51:42 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 14:03:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "image_buffer.h"

namespace vox::gfx {

class TextureSampler;

class GBufferSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
        PositionTexture,
        NormalTexture,
        AlbedoTexture,

        Count
    };

    enum class Texture: u32 {
        PositionTexture,
        NormalTexture,
        AlbedoTexture,

        Count
    };

    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = DescriptorSet;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GBufferSet(): super(DescriptorSetIndex::GBuffer) {}

    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device) override;

    /* ====================================================================== */

    const ImageBuffer&  getImageBuffer(const u32 index) const noexcept override;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 BINDING_COUNT = (u32)BindingIndex::Count;
    static constexpr u32 TEXTURE_COUNT = (u32)Texture::Count;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::array<TextureSampler*, TEXTURE_COUNT>  m_textures;

}; // class GBufferSet

} // namespace vox::gfx