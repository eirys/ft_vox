/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:58:48 by etran             #+#    #+#             */
/*   Updated: 2024/06/02 01:36:42 by etran            ###   ########.fr       */
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

u32 VertexBuffer::ms_maxVertexInstanceCount = 0;

#if ENABLE_FRUSTUM_CULLING
/**
 * @brief Creates a vertex buffer.
 */
void VertexBuffer::init(
    const Device& device,
    const game::GameState& gameState
) {
    BufferMetadata metadata{};
    metadata.m_format = sizeof(VertexInstance);
    metadata.m_size = ms_maxVertexInstanceCount;
    metadata.m_usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    metadata.m_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    m_buffer.init(device, std::move(metadata));
    m_buffer.map(device);
}

void VertexBuffer::update(const Device& device, const game::GameState& gameState) {
    u32 visibleAABBs = 0;
    const auto instances = _computeVertexInstances(gameState, visibleAABBs);

    if (visibleAABBs == m_visibleAABBsCount)
        return;
    m_visibleAABBsCount = visibleAABBs;
    m_instancesCount = instances.size();
    m_buffer.copyFrom(instances.data(), sizeof(VertexInstance) * m_instancesCount, 0);
}

#else

void VertexBuffer::init(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const game::GameState& gameState
) {
    const auto instances = _computeVertexInstances(gameState);
    m_instancesCount = instances.size();

    BufferMetadata metadata{};
    metadata.m_format = sizeof(VertexInstance);
    metadata.m_size = m_instancesCount;
    metadata.m_usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    metadata.m_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    m_buffer.init(device, std::move(metadata));

    Buffer  stagingBuffer = m_buffer.createStagingBuffer(device);
    stagingBuffer.map(device);
    stagingBuffer.copyFrom(instances.data(), sizeof(VertexInstance) * m_instancesCount, 0);
    stagingBuffer.unmap(device);

    cmdBuffer->reset();
    cmdBuffer->startRecording();
    m_buffer.copyBuffer(cmdBuffer, stagingBuffer);
    cmdBuffer->stopRecording();
    cmdBuffer->awaitEndOfRecording(device);

    stagingBuffer.destroy(device);
    LINFO("Vertex buffer initialized.");
}

#endif

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

enum ChunkNeighbor: u32 {
    Left = 0,
    Right,
    Front,
    Back,

    Count
};

static
bool _isExposed(
    const game::Chunk* neighbor,
    const u32 checkedX,
    const u32 checkedY,
    const u32 checkedZ
) {
    return (neighbor && neighbor->getBlock(checkedX, checkedY, checkedZ).isVoid()) || neighbor == nullptr;
}

static
void _evaluateChunk(
    const game::Chunk& chunk,
    std::vector<VertexInstance>& instances,
    const std::array<const game::Chunk*, ChunkNeighbor::Count>& neighbors // Don't count top/bottom yet
) {
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

                // Check if block face is visible
                if (y == UPPER_LIMIT || chunk.getBlock(x, y + 1, z).isVoid()) {
                    instances.emplace_back(game::BlockFace::Top, block.getTextureId(game::BlockFace::Top), blockId, chunkId);
                }
                if (y == LOWER_LIMIT || chunk.getBlock(x, y - 1, z).isVoid()) {
                    instances.emplace_back(game::BlockFace::Bottom, block.getTextureId(game::BlockFace::Bottom), blockId, chunkId);
                }

                if ((x != UPPER_LIMIT && chunk.getBlock(x + 1, y, z).isVoid()) || (x == UPPER_LIMIT && _isExposed(neighbors[ChunkNeighbor::Right], LOWER_LIMIT, y, z)))
                    instances.emplace_back(game::BlockFace::Right, block.getTextureId(game::BlockFace::Right), blockId, chunkId);
                if ((x != LOWER_LIMIT && chunk.getBlock(x - 1, y, z).isVoid()) || (x == LOWER_LIMIT && _isExposed(neighbors[ChunkNeighbor::Left], UPPER_LIMIT, y, z)))
                    instances.emplace_back(game::BlockFace::Left, block.getTextureId(game::BlockFace::Left), blockId, chunkId);
                if ((z != UPPER_LIMIT && chunk.getBlock(x, y, z + 1).isVoid()) || (z == UPPER_LIMIT && _isExposed(neighbors[ChunkNeighbor::Front], x, y, LOWER_LIMIT)))
                    instances.emplace_back(game::BlockFace::Front, block.getTextureId(game::BlockFace::Front), blockId, chunkId);
                if ((z != LOWER_LIMIT && chunk.getBlock(x, y, z - 1).isVoid()) || (z == LOWER_LIMIT && _isExposed(neighbors[ChunkNeighbor::Back], x, y, UPPER_LIMIT)))
                    instances.emplace_back(game::BlockFace::Back, block.getTextureId(game::BlockFace::Back), blockId, chunkId);
            }
        }
    }
}

#if ENABLE_FRUSTUM_CULLING

std::vector<VertexInstance> VertexBuffer::_computeVertexInstances(
    const game::GameState& gameState,
    u32& visibleAABBs
) const {
    std::vector<VertexInstance> instances;

    const BoundingFrustum frustum(gameState.getController().getCamera());

    // Retrieve blocks and cull invisible faces
    for (u32 z = 0; z < RENDER_DISTANCE; ++z) {
        for (u32 x = 0; x < RENDER_DISTANCE; ++x) {
            for (u32 y = 0; y < RENDER_HEIGHT; ++y) {
                auto& chunk = gameState.getWorld().getChunk(x, y, z);

                std::array<const game::Chunk*, ChunkNeighbor::Count>  neighbors = {nullptr, nullptr, nullptr, nullptr};
                if (x > 0)
                    neighbors[ChunkNeighbor::Left] = &gameState.getWorld().getChunk(x - 1, y, z);
                if (x < RENDER_DISTANCE - 1)
                    neighbors[ChunkNeighbor::Right] = &gameState.getWorld().getChunk(x + 1, y, z);
                if (z > 0)
                    neighbors[ChunkNeighbor::Back] = &gameState.getWorld().getChunk(x, y, z - 1);
                if (z < RENDER_DISTANCE - 1)
                    neighbors[ChunkNeighbor::Front] = &gameState.getWorld().getChunk(x, y, z + 1);

                const BoundingBox AABB = chunk.getBoundingBox();

                if (AABB.isVisible(frustum)) {
                    ++visibleAABBs;
                    _evaluateChunk(chunk, instances, neighbors);
                }
            }
        }
    }
    return instances;
}

#else

std::vector<VertexInstance> VertexBuffer::_computeVertexInstances(const game::GameState& gameState) const {
    std::vector<VertexInstance> instances;

    // Retrieve blocks and cull invisible faces
    for (u32 z = 0; z < RENDER_DISTANCE; ++z) {
        for (u32 x = 0; x < RENDER_DISTANCE; ++x) {
            for (u32 y = 0; y < RENDER_HEIGHT; ++y) {
                auto& chunk = gameState.getWorld().getChunk(x, y, z);

                std::array<const game::Chunk*, ChunkNeighbor::Count>  neighbors = {nullptr, nullptr, nullptr, nullptr};
                if (x > 0)
                    neighbors[ChunkNeighbor::Left] = &gameState.getWorld().getChunk(x - 1, y, z);
                if (x < RENDER_DISTANCE - 1)
                    neighbors[ChunkNeighbor::Right] = &gameState.getWorld().getChunk(x + 1, y, z);
                if (z > 0)
                    neighbors[ChunkNeighbor::Back] = &gameState.getWorld().getChunk(x, y, z - 1);
                if (z < RENDER_DISTANCE - 1)
                    neighbors[ChunkNeighbor::Front] = &gameState.getWorld().getChunk(x, y, z + 1);

                _evaluateChunk(chunk, instances, neighbors);
            }
        }
    }
    return instances;
}

#endif

/* ========================================================================== */
/*                                    OTHER                                   */
/* ========================================================================== */

static
u32 _evaluateChunkInstances(
    const game::Chunk& chunk,
    const std::array<const game::Chunk*, ChunkNeighbor::Count>& neighbors // Don't count top/bottom yet
) {
    constexpr u32 UPPER_LIMIT = CHUNK_SIZE - 1;
    constexpr u32 LOWER_LIMIT = 0;

    u32 instances = 0;

    for (u32 z = 0; z < CHUNK_SIZE; ++z) {
        for (u32 x = 0; x < CHUNK_SIZE; ++x) {
            for (u32 y = 0; y < CHUNK_SIZE; ++y) {
                if (chunk.getBlock(x, y, z).isVoid())
                    continue;

                // Check if block face is visible
                if (y == UPPER_LIMIT || chunk.getBlock(x, y + 1, z).isVoid()) ++instances;
                if (y == LOWER_LIMIT || chunk.getBlock(x, y - 1, z).isVoid()) ++instances;

                if ((x != UPPER_LIMIT && chunk.getBlock(x + 1, y, z).isVoid()) || (x == UPPER_LIMIT && _isExposed(neighbors[ChunkNeighbor::Right], LOWER_LIMIT, y, z)))
                    ++instances;
                if ((x != LOWER_LIMIT && chunk.getBlock(x - 1, y, z).isVoid()) || (x == LOWER_LIMIT && _isExposed(neighbors[ChunkNeighbor::Left], UPPER_LIMIT, y, z)))
                    ++instances;
                if ((z != UPPER_LIMIT && chunk.getBlock(x, y, z + 1).isVoid()) || (z == UPPER_LIMIT && _isExposed(neighbors[ChunkNeighbor::Front], x, y, LOWER_LIMIT)))
                    ++instances;
                if ((z != LOWER_LIMIT && chunk.getBlock(x, y, z - 1).isVoid()) || (z == LOWER_LIMIT && _isExposed(neighbors[ChunkNeighbor::Back], x, y, UPPER_LIMIT)))
                    ++instances;
            }
        }
    }
    return instances;
}

void VertexBuffer::computeMaxVertexInstanceCount(const game::GameState& gameState) {
    std::array<const game::Chunk*, ChunkNeighbor::Count>  neighbors = {nullptr, nullptr, nullptr, nullptr};

    for (u32 z = 0; z < RENDER_DISTANCE; ++z) {
        for (u32 x = 0; x < RENDER_DISTANCE; ++x) {
            for (u32 y = 0; y < RENDER_HEIGHT; ++y) {
                auto& chunk = gameState.getWorld().getChunk(x, y, z);

                std::array<const game::Chunk*, ChunkNeighbor::Count>  neighbors = {nullptr, nullptr, nullptr, nullptr};
                if (x > 0)
                    neighbors[ChunkNeighbor::Left] = &gameState.getWorld().getChunk(x - 1, y, z);
                if (x < RENDER_DISTANCE - 1)
                    neighbors[ChunkNeighbor::Right] = &gameState.getWorld().getChunk(x + 1, y, z);
                if (z > 0)
                    neighbors[ChunkNeighbor::Back] = &gameState.getWorld().getChunk(x, y, z - 1);
                if (z < RENDER_DISTANCE - 1)
                    neighbors[ChunkNeighbor::Front] = &gameState.getWorld().getChunk(x, y, z + 1);

                ms_maxVertexInstanceCount += _evaluateChunkInstances(chunk, neighbors);
            }
        }
    }
}

} // namespace vox::gfx