/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:02:25 by etran             #+#    #+#             */
/*   Updated: 2024/05/28 15:01:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include <array>

#include "types.h"

namespace vox::gfx {

enum class BlockFace: u8 {
    Top = 0,
    Bottom,
    Left,
    Right,
    Front,
    Back
};

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

    VertexInstance(const BlockFace face, const u16 blockId, const u16 chunkId);

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