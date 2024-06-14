/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pfd_set.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:38:00 by etran             #+#    #+#             */
/*   Updated: 2024/06/13 23:54:27 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "pfd_ubo.h"
#include "buffer.h"
#include "texture_sampler.h"
#include "vox_decl.h"

namespace vox::gfx {

class PFDSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
        GameData,
#if ENABLE_SHADOW_MAPPING
        ProjectorViewProj,
        Shadowmap,
#endif

        Count
    };

    enum class Texture: u32 {
#if ENABLE_SHADOW_MAPPING
        Shadowmap,
#endif

        Count
    };

    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = DescriptorSet;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    PFDSet(): super(DescriptorSetIndex::Pfd) {}

    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device) override;
    void    update(const game::GameState& state);

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
    Buffer  m_mvpDataBuffer;
    PFDUbo  m_data;

};

} // namespace vox::gfx