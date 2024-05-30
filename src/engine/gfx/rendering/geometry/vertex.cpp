/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:03:25 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 17:07:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vertex.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   VERTEX                                   */
/* ========================================================================== */

VertexInstance::VertexInstance(
    const game::BlockFace face,
    const u8 textureId,
    const u16 blockId,
    const u16 chunkId
) {
    m_data = ((u8)textureId << 29) | ((u8)face << 26) | (blockId << 14) | chunkId;
}

VertexInstance::BindingsDescription VertexInstance::getBindingDescriptions() noexcept {
    BindingsDescription bindingDescriptions{};

    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = getStride();
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
    // BEWARE!!! We are using instanced rendering, so we need to set the input rate
    // to VK_VERTEX_INPUT_RATE_INSTANCE

    return bindingDescriptions;
}

VertexInstance::AttributesDescription VertexInstance::getAttributeDescriptions() noexcept {
    AttributesDescription attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32_UINT;
    attributeDescriptions[0].offset = 0;

    return attributeDescriptions;
}

u32 VertexInstance::getStride() noexcept {
    return sizeof(VertexInstance);
}

} // namespace vox::gfx