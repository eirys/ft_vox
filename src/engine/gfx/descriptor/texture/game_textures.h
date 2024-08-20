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

class GameTexture final: public Texture {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Texture;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GameTexture(): super(true) {}

    ~GameTexture() = default;

    GameTexture(GameTexture&& other) = delete;
    GameTexture(const GameTexture& other) = delete;
    GameTexture& operator=(GameTexture&& other) = delete;
    GameTexture& operator=(const GameTexture& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer) override;

}; // class GameTexture

} // namespace vox::gfx
