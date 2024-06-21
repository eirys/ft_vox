/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pfd_set.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:38:00 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 03:42:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "buffer.h"
#include "vox_decl.h"
#include "matrix.h"
#include "vector.h"

namespace game {
class GameState;
}

namespace vox::gfx {

struct PFDUbo final {
    struct {
        math::Vect2 sunPos;
        u32         skyHue;
        u32         debugIndex;
        u32         padding[12];
    }           m_gameData;

#if ENABLE_SHADOW_MAPPING
    math::Mat4  m_projectorViewProj;
#endif
}; // struct PFDUbo

static_assert(sizeof(PFDUbo) % 0x40 == 0, "PFD UBO size is must be a multiple of 64 bytes. Check value of PADDING_NEEDED.");
constexpr u32 PADDING_NEEDED = (0x40 - (sizeof(PFDUbo) % 0x40)) / sizeof(u32);


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
    void    update(const game::GameState& state);

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 BINDING_COUNT = (u32)BindingIndex::Count;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Buffer  m_mvpDataBuffer;
    PFDUbo  m_data;

}; // class PFDSet

} // namespace vox::gfx