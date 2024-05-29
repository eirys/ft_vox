/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:34:05 by etran             #+#    #+#             */
/*   Updated: 2024/05/28 16:18:18 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>
#include <vector>
#include <stack>

#include "buffer.h"
#include "vertex.h"

namespace vox::gfx {

class VertexBuffer final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    init(
        const Device& device,
        const ICommandBuffer* cmdBuffer,
        const std::vector<VertexInstance>& instances);
    void    destroy(const Device& device);

    u32     getInstancesCount() const noexcept;

    /* ====================================================================== */

    const Buffer& getBuffer() const;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Buffer  m_buffer;
    u32     m_instancesCount = 0;

}; // class ChunkVertexBuffer

} // namespace vox::gfx