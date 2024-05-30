/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:02:25 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 17:07:34 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include <array>

#include "block_decl.h"

namespace vox::gfx {

class VertexInstance final {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using AttributesDescription = std::array<VkVertexInputAttributeDescription, 1>;
    using BindingsDescription = std::array<VkVertexInputBindingDescription, 1>;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    VertexInstance(
        const game::BlockFace face,
        const u8 textureId,
        const u16 blockId,
        const u16 chunkId);

    VertexInstance() = default;
    ~VertexInstance() = default;
    VertexInstance(VertexInstance&& other) = default;
    VertexInstance& operator=(VertexInstance&& other) = default;

    VertexInstance(const VertexInstance& other) = delete;
    VertexInstance& operator=(const VertexInstance& other) = delete;

    /* ====================================================================== */

    static BindingsDescription      getBindingDescriptions() noexcept;
    static AttributesDescription    getAttributeDescriptions() noexcept;
    static u32                      getStride() noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    u32 m_data = 0;

}; // class VertexInstance

} // namespace vox::gfx