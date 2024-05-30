/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pfd_set.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:38:00 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 16:13:26 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "pfd_ubo.h"
#include "buffer.h"
#include "game_texture_sampler.h"

namespace vox::gfx {

class PFDSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
        ViewProj = 0,
        GameData,

        First = ViewProj,
        Last = GameData
    };

    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = DescriptorSet;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    PFDSet(): super(DescriptorSetIndex::Pfd) {}

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device) override;
    void    update(const game::GameState& state);

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 BINDING_COUNT = enumSize<BindingIndex>();

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Buffer  m_mvpDataBuffer;
    PFDUbo  m_data;

};

} // namespace vox::gfx