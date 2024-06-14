/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_set.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:21:18 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 14:03:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "texture_sampler.h"
#include "game_decl.h"
#include "vox_decl.h"

#include <array>

namespace vox::gfx {

class WorldSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
        Textures,
        Noise,

#if ENABLE_CUBEMAP
        Cubemap,
#endif

        Count
    };

    enum class Texture: u32 {
        GameTexture,
        PerlinNoise,

#if ENABLE_CUBEMAP
        Cubemap,
#endif

        Count
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    WorldSet(): DescriptorSet(DescriptorSetIndex::WorldData) {}

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

    std::array<TextureSampler*, TEXTURE_COUNT> m_textures;

}; // class WorldSet

} // namespace vox::gfx