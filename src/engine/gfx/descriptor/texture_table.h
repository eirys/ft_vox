/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_table.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:31:01 by etran             #+#    #+#             */
/*   Updated: 2024/06/20 16:21:35 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>
#include <vector>

#include "texture_decl.h"
#include "sampler.h"

namespace vox::gfx {

class Texture;
class ICommandBuffer;

class TextureTable final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    TextureTable() = default;
    ~TextureTable() = default;

    TextureTable(TextureTable&& other) = delete;
    TextureTable(const TextureTable& other) = delete;
    TextureTable& operator=(TextureTable&& other) = delete;
    TextureTable& operator=(const TextureTable& other) = delete;

    /* ====================================================================== */

    static void init(const Device& device, const ICommandBuffer* cmdBuffer);
    static void destroy(const Device& device);

    /* ====================================================================== */

    static const Texture*   getTexture(const TextureIndex index) noexcept;

    static const Sampler&   getSampler(const Device& device, const Sampler::Metadata& metadata);
    static const Sampler&   getSampler(
        const Device& device,
        const Sampler::Filter filter,
        const Sampler::Border border,
        const Sampler::BorderColor borderColor);

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static std::array<Texture*, TEXTURE_COUNT> ms_textures;
    static std::vector<Sampler>                ms_samplers;

}; // class TextureTable

} // namespace vox::gfx