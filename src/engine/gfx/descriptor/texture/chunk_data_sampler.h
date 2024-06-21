/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_data_sampler.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 23:04:53 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:30:22 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture.h"

namespace vox::gfx {

class ChunkDataSampler final: public Texture {
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

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

// private:
//     /* ====================================================================== */
//     /*                                 METHODS                                */
//     /* ====================================================================== */

//     void    _createSampler(const Device& device);

}; // class ChunkDataSampler

} // namespace vox::gfx