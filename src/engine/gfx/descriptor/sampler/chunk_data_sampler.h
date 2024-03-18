/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_data_sampler.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 23:04:53 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 10:36:23 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture_sampler.h"

namespace vox::gfx {

class ChunkDataSampler final: public TextureSampler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    ChunkDataSampler() = default;
    ~ChunkDataSampler() = default;

    ChunkDataSampler(ChunkDataSampler&& other) = delete;
    ChunkDataSampler(const ChunkDataSampler& other) = delete;
    ChunkDataSampler& operator=(ChunkDataSampler&& other) = delete;
    ChunkDataSampler& operator=(const ChunkDataSampler& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data) override;

private:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createSampler(const Device& device);

}; // class ChunkDataSampler

} // namespace vox::gfx