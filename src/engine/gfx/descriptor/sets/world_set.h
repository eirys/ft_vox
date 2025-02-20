/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_set.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:21:18 by etran             #+#    #+#             */
/*   Updated: 2024/09/11 13:13:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "game_decl.h"
#include "vox_decl.h"
#include "buffer.h"

namespace vox::gfx {

class WorldSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct Ubo {
        enum: u32 {
            WorldSide,
            FogDistance,
            WorldPortionOffsetX,
            WorldPortionOffsetZ,
            RenderOffsetX,
            RenderOffsetZ,
        };
        u32 data[16];
    };

    // constexpr u32 PADDING_NEEDED = (0x40 - (sizeof(Ubo) % 0x40)) / sizeof(u32);

    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
        Textures,
        Noise,
        Chunks,
        RenderData,

#if ENABLE_CUBEMAP
        Cubemap,
#endif

        Count
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device) override;
    void    update(const Device& device, const ICommandBuffer* cmdBuffer);

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 BINDING_COUNT = (u32)BindingIndex::Count;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Buffer  m_renderDataBuffer;
    Buffer  m_stagingBuffer;
    Ubo     m_ubo;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void _sendData(const Device& device, const ICommandBuffer* cmdBuffer);

}; // class WorldSet

static_assert(sizeof(WorldSet::Ubo) % 0x40 == 0, "PFD UBO size is must be a multiple of 64 bytes. Check value of PADDING_NEEDED.");

} // namespace vox::gfx