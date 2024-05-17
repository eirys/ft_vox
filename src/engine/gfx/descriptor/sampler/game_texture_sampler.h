/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_texture_sampler.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 17:33:50 by etran             #+#    #+#             */
/*   Updated: 2024/03/27 15:27:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "texture_sampler.h"

namespace vox::gfx {

class GameTextureSampler final: public TextureSampler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GameTextureSampler() = default;
    ~GameTextureSampler() = default;

    GameTextureSampler(GameTextureSampler&& other) = delete;
    GameTextureSampler(const GameTextureSampler& other) = delete;
    GameTextureSampler& operator=(GameTextureSampler&& other) = delete;
    GameTextureSampler& operator=(const GameTextureSampler& other) = delete;

    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device, const ICommandBuffer* cmdBuffer, const void* data = nullptr) override;

private:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createSampler(const Device& device);

}; // class GameTextureSampler

} // namespace vox::gfx
