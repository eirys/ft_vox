/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_data_texture.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 23:04:53 by etran             #+#    #+#             */
/*   Updated: 2024/07/12 18:31:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture.h"

namespace vox::gfx {

class ChunkDataTexture final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    ChunkDataTexture(): super(false) {}

    ~ChunkDataTexture() = default;

    ChunkDataTexture(ChunkDataTexture&& other) = delete;
    ChunkDataTexture(const ChunkDataTexture& other) = delete;
    ChunkDataTexture& operator=(ChunkDataTexture&& other) = delete;
    ChunkDataTexture& operator=(const ChunkDataTexture& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer) override;

}; // class ChunkDataTexture

} // namespace vox::gfx