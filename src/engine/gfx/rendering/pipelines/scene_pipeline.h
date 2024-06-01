/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 23:49:38 by etran             #+#    #+#             */
/*   Updated: 2024/06/02 01:24:46 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"
#include "vertex_buffer.h"
#include "game_decl.h"

namespace game {
class GameState;
}

namespace vox::gfx {

class ICommandBuffer;

class ScenePipeline final: public Pipeline {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = Pipeline;

    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class ShaderStage: u32 {
        VertexInstance = 0,
        Fragment,

        First = VertexInstance,
        Last = Fragment
    };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    ScenePipeline() = default;
    ~ScenePipeline() = default;

    ScenePipeline(ScenePipeline&& other) = delete;
    ScenePipeline(const ScenePipeline& other) = delete;
    ScenePipeline& operator=(ScenePipeline&& other) = delete;
    ScenePipeline& operator=(const ScenePipeline& other) = delete;

    /* ====================================================================== */

    void    init(
        const Device& device,
        const VkRenderPass& renderPass,
        const VkPipelineLayout& pipelineLayout) override;
    void    destroy(const Device& device) override;

    void    record(
        const VkPipelineLayout layout,
        const DescriptorTable& descriptorTable,
        const ICommandBuffer* cmdBuffer) override;

#if ENABLE_FRUSTUM_CULLING
    void    initVertexBuffer(
        const Device& device,
        const game::GameState& gameState);
    void    updateVertexBuffer(
        const Device& device,
        const game::GameState& gameState);
#else
    void    initVertexBuffer(
        const Device& device,
        const ICommandBuffer* cmdBuffer,
        const game::GameState& gameState);
#endif

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32    SHADER_STAGE_COUNT = enumSize<ShaderStage>();
    static constexpr u32    VERTEX_BUFFER_COUNT = 1;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::array<VertexBuffer, VERTEX_BUFFER_COUNT>   m_vertexBuffers;
    u32                                             m_currentBuffer = 0;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    VertexBuffer&   _getCurrentBuffer();
    void            _switchBuffer();

}; // class ScenePipeline

} // namespace vox::gfx
