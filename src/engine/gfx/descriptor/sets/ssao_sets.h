/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssao_sets.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 13:53:02 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:29:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "vector.h"
#include "buffer.h"
#include "game_decl.h"

namespace vox::gfx {

class Buffer;
class Texture;

class SSAOSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct SSAOUbo {
        math::Vect3 m_samples[SSAO_KERNEL_SIZE];
    };

    static_assert(sizeof(SSAOUbo) % 0x40 == 0, "PFD UBO size is must be a multiple of 64 bytes. Check value of PADDING_NEEDED.");
    static constexpr u32 PADDING_NEEDED = (0x40 - (sizeof(SSAOUbo) % 0x40)) / sizeof(u32);

    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
        Samples,
        PositionTexture,
        NormalViewTexture,
        NoiseTexture,

        Count
    };

    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = DescriptorSet;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device) override;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 BINDING_COUNT = (u32)BindingIndex::Count;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Buffer  m_samplesBuffer;
    SSAOUbo m_data;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void _fillBuffer(const Device& device, const ICommandBuffer* transferBuffer);

}; // class SSAOSet


class SSAOBlurSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
        Texture,

        Count
    };

    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = DescriptorSet;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device) override;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 BINDING_COUNT = (u32)BindingIndex::Count;

}; // class SSAOBlurSet

} // namespace vox::gfx