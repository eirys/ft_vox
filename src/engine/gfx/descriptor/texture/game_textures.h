/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_textures.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:33:50 by etran             #+#    #+#             */
/*   Updated: 2024/06/18 15:47:49 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture.h"

namespace vox::gfx {

class GameTextureSampler final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GameTextureSampler(): super(true) {}

    ~GameTextureSampler() = default;

    GameTextureSampler(GameTextureSampler&& other) = delete;
    GameTextureSampler(const GameTextureSampler& other) = delete;
    GameTextureSampler& operator=(GameTextureSampler&& other) = delete;
    GameTextureSampler& operator=(const GameTextureSampler& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

}; // class GameTextureSampler

} // namespace vox::gfx
