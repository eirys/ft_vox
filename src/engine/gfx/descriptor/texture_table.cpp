/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_table.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:21:04 by etran             #+#    #+#             */
/*   Updated: 2024/06/25 14:50:11 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture_table.h"
#include "device.h"

#include "game_textures.h"
#include "perlin_noise_texture.h"
#include "gbuffer_textures.h"

#if ENABLE_SHADOW_MAPPING
#include "shadowmap_texture.h"
#endif

#if ENABLE_CUBEMAP
#include "skybox_texture.h"
#endif

#include "debug.h"

namespace vox::gfx {

std::array<Texture*, TEXTURE_COUNT> TextureTable::ms_textures;
std::vector<Sampler>                TextureTable::ms_samplers;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void TextureTable::init(const Device& device, const ICommandBuffer* cmdBuffer) {
    ms_textures[(u32)TextureIndex::GameTexture] = new GameTextureSampler();
    ms_textures[(u32)TextureIndex::PerlinNoise] = new PerlinNoiseSampler();

    ms_textures[(u32)TextureIndex::GBufferPosition] = new PositionTexture();
    ms_textures[(u32)TextureIndex::GBufferNormal] = new NormalTexture();
    ms_textures[(u32)TextureIndex::GBufferAlbedo] = new AlbedoTexture();
    ms_textures[(u32)TextureIndex::GBufferDepth] = new DepthTexture();

#if ENABLE_SSAO
    ms_textures[(u32)TextureIndex::GBufferNormalView] = new NormalViewTexture();
    ms_textures[(u32)TextureIndex::GBufferPositionView] = new PositionViewTexture();
    ms_textures[(u32)TextureIndex::GBufferSSAO] = new SSAOTexture();
    ms_textures[(u32)TextureIndex::GBufferSSAOBlur] = new SSAOBlurTexture();
#endif
#if ENABLE_CUBEMAP
    ms_textures[(u32)TextureIndex::SkyboxSampler] = new SkyboxSampler();
#endif
#if ENABLE_SHADOW_MAPPING
    ms_textures[(u32)TextureIndex::ShadowMap] = new ShadowmapSampler();
#endif

    for (auto texture: ms_textures) texture->init(device);

    ms_textures[(u32)TextureIndex::GameTexture]->fill(device, cmdBuffer);
    ms_textures[(u32)TextureIndex::PerlinNoise]->fill(device, cmdBuffer);
#if ENABLE_CUBEMAP
    ms_textures[(u32)TextureIndex::Skybox]->fill(device, cmdBuffer);
#endif

    LDEBUG("Texture table: created textures");
}

void TextureTable::destroy(const Device& device) {
    for (u32 i = 0; i < TEXTURE_COUNT; ++i) {
        ms_textures[i]->destroy(device);
        delete ms_textures[i];
    }
    for (auto& sampler: ms_samplers) {
        vkDestroySampler(device.getDevice(), sampler.getSampler(), nullptr);
    }
}

/* ========================================================================== */

const Texture* TextureTable::getTexture(const TextureIndex index) noexcept {
    return ms_textures[(u32)index];
}

static
Sampler _createNewSampler(const Device& device, const Sampler::Metadata& data) {
    Sampler sampler{};
    sampler.init(device, data);

    LDEBUG("Created new sampler: " <<
        "filter: " << (u32)data.m_filter << ", " <<
        "border: " << (u32)data.m_border << ", " <<
        "borderColor: " << (u32)data.m_borderColor
    );
    return sampler;
}

const Sampler& TextureTable::getSampler(
    const Device& device,
    const Sampler::Filter filter,
    const Sampler::Border border,
    const Sampler::BorderColor borderColor
) {
    for (const auto& sampler: ms_samplers) {
        const Sampler::Metadata& samplerProperties = sampler.getMetadata();

        if (samplerProperties.m_filter == filter &&
            samplerProperties.m_border == border &&
            samplerProperties.m_borderColor == borderColor &&
            samplerProperties.m_compare == Sampler::Compare::Never &&
            samplerProperties.m_enableAnisotropy == false) {
            return sampler;
        }
    }

    Sampler::Metadata metadata{};
    metadata.m_filter = filter;
    metadata.m_border = border;
    metadata.m_borderColor = borderColor;

    return ms_samplers.emplace_back(_createNewSampler(device, metadata));
}

const Sampler& TextureTable::getSampler(const Device& device, const Sampler::Metadata& metadata) {
    for (const auto& sampler: ms_samplers)
        if (sampler.getMetadata() == metadata) return sampler;

    return ms_samplers.emplace_back(_createNewSampler(device, metadata));
}

} // namespace vox::gfx