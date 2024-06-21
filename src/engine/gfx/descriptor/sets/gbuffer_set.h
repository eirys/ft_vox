/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gbuffer_set.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 23:51:42 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 01:33:27 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "descriptor_set.h"

namespace vox::gfx {

class Texture;

class GBufferSet final: public DescriptorSet {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class BindingIndex: u32 {
        PositionTexture,
        NormalTexture,
        AlbedoTexture,
        NormalViewTexture,

#if ENABLE_SSAO
        SsaoTexture,
        SsaoBlur,
#endif

        Count
    };

    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = DescriptorSet;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    init(const Device& device, const ICommandBuffer* cmdBuffer) override;
    void    destroy(const Device& device) override;

    void    fill(const Device& device) override;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 BINDING_COUNT = (u32)BindingIndex::Count;

}; // class GBufferSet

} // namespace vox::gfx