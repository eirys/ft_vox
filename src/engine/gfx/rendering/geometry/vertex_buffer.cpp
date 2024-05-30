/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:58:48 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 22:47:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vertex_buffer.h"
#include "icommand_buffer.h"
#include "game_state.h"
#include "bounding_box.h"
#include "bounding_frustum.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

/**
 * @brief Creates a vertex buffer.
 */
void VertexBuffer::init(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const game::GameState& gameState
) {
    std::vector<VertexInstance> instances = _computeVerticeInstances(gameState);
    m_instancesCount = instances.size();

    // BufferMetadata metadata{};
    // metadata.m_size = sizeof(VertexInstance) * m_instancesCount;
    // metadata.m_usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    // metadata.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    // m_buffer.init(device, std::move(metadata));
    // m_buffer.map(device);
    // m_buffer.copyFrom(instances.data());
    // m_buffer.unmap(device);

    BufferMetadata metadata{};
    metadata.m_size = sizeof(VertexInstance) * m_instancesCount;
    metadata.m_usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    metadata.m_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    m_buffer.init(device, std::move(metadata));

    // Vertex buffer is static. Create staging buffer and copy data to it for
    // better performance.
    Buffer stagingBuffer = m_buffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(instances.data());
    stagingBuffer.unmap(device);

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    m_buffer.copyBuffer(cmdBuffer, stagingBuffer);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);
    stagingBuffer.destroy(device);
}

void VertexBuffer::destroy(const Device& device) {
    m_buffer.destroy(device);
}

/* ========================================================================== */

const Buffer& VertexBuffer::getBuffer() const {
    return m_buffer;
}

u32 VertexBuffer::getInstancesCount() const noexcept {
    return m_instancesCount;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

std::vector<VertexInstance> VertexBuffer::_computeVerticeInstances(const game::GameState& gameState) const {
    std::vector<VertexInstance> instances;

    // const BoundingFrustum frustum(gameState.getController().getCamera());

    // Retrieve blocks and cull invisible faces
    for (u32 z = 0; z < RENDER_DISTANCE; ++z) {
        for (u32 x = 0; x < RENDER_DISTANCE; ++x) {
            for (u32 y = 0; y < RENDER_HEIGHT; ++y) {
                auto& chunk = gameState.getWorld().getChunk(x, y, z);

                // const BoundingBox AABB(
                //     math::Vect3(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE),
                //     math::Vect3(CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f));

                // if (AABB.checkIntersection(frustum) != IntersectionType::Outside)
                    _evaluateChunk(chunk, instances);
            }
        }
    }
    return instances;
}

void VertexBuffer::_evaluateChunk(const game::Chunk& chunk, std::vector<VertexInstance>& instances) const {
    constexpr u32 UPPER_LIMIT = CHUNK_SIZE - 1;
    constexpr u32 LOWER_LIMIT = 0;

    for (u32 z = 0; z < CHUNK_SIZE; ++z) {
        for (u32 x = 0; x < CHUNK_SIZE; ++x) {
            for (u32 y = 0; y < CHUNK_SIZE; ++y) {
                const auto& block = chunk.getBlock(x, y, z);

                if (block.isVoid())
                    continue;

                const u16 blockId = (x << 8) | (y << 4) | z;
                const u16 chunkId = chunk.getId();

                // TODO: add frustum culling

                // Check if block face is visible
                if (y == UPPER_LIMIT || chunk.getBlock(x, y + 1, z).isVoid()) {
                    instances.emplace_back(game::BlockFace::Top, block.getTextureId(game::BlockFace::Top), blockId, chunkId);
                }
                if (y == LOWER_LIMIT || chunk.getBlock(x, y - 1, z).isVoid()) {
                    instances.emplace_back(game::BlockFace::Bottom, block.getTextureId(game::BlockFace::Bottom), blockId, chunkId);
                }
                if (x == UPPER_LIMIT || chunk.getBlock(x + 1, y, z).isVoid()) {
                    instances.emplace_back(game::BlockFace::Right, block.getTextureId(game::BlockFace::Right), blockId, chunkId);
                }
                if (x == LOWER_LIMIT || chunk.getBlock(x - 1, y, z).isVoid()) {
                    instances.emplace_back(game::BlockFace::Left, block.getTextureId(game::BlockFace::Left), blockId, chunkId);
                }
                if (z == UPPER_LIMIT || chunk.getBlock(x, y, z + 1).isVoid()) {
                    instances.emplace_back(game::BlockFace::Front, block.getTextureId(game::BlockFace::Front), blockId, chunkId);
                }
                if (z == LOWER_LIMIT || chunk.getBlock(x, y, z - 1).isVoid()) {
                    instances.emplace_back(game::BlockFace::Back, block.getTextureId(game::BlockFace::Back), blockId, chunkId);
                }
            }
        }
    }
}

} // namespace vox::gfx