/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_set.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:21:18 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 13:45:44 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"
#include "chunk_data_sampler.h"
#include "game_texture_sampler.h"

namespace vox::gfx {

class WorldSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
        BlockPos = 0,
        Textures,

        First = BlockPos,
        Last = Textures
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    WorldSet(): DescriptorSet(DescriptorSetIndex::WorldData) {}

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device) override;
    void    update(const Device& device, const game::GameState& state, const ICommandBuffer* cmdBuffer);

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 BINDING_COUNT = enumSize<BindingIndex>();

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    ChunkDataSampler    m_chunkDataSampler;
    GameTextureSampler  m_gameTextureSampler;

}; // class WorldSet

} // namespace vox::gfx