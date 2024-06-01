/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 09:48:27 by etran             #+#    #+#             */
/*   Updated: 2024/06/02 01:24:52 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_pipeline.h"
#include "device.h"
#include "icommand_buffer.h"
#include "descriptor_table.h"

#include <array>
#include <stdexcept>

#include "debug.h"

namespace vox::gfx {

enum class SceneDescriptorSet: u32 {
    PerFrameData = 0,
    Textures,

    First = PerFrameData,
    Last = Textures
};

enum class SetIndex: u32 {
    PerFrameData    = (u32)DescriptorSetIndex::Pfd,
    Textures        = (u32)DescriptorSetIndex::WorldData,

    First = PerFrameData,
    Last = Textures
};


static constexpr u32 DESCRIPTOR_SET_COUNT = enumSize<SceneDescriptorSet>();

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void ScenePipeline::init(
    const Device& device,
    const VkRenderPass& renderPass,
    const VkPipelineLayout& pipelineLayout
) {
    VkPipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInput.vertexBindingDescriptionCount = VertexInstance::getBindingDescriptions().size();
    vertexInput.pVertexBindingDescriptions = VertexInstance::getBindingDescriptions().data();
    vertexInput.vertexAttributeDescriptionCount = VertexInstance::getAttributeDescriptions().size();
    vertexInput.pVertexAttributeDescriptions = VertexInstance::getAttributeDescriptions().data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    #ifdef __DEBUG
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    #else
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    #endif
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisample{};
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.sampleShadingEnable = VK_FALSE;
    multisample.minSampleShading = 1.0f;
    multisample.pSampleMask = nullptr;
    multisample.rasterizationSamples = device.getMsaaCount();
    multisample.alphaToCoverageEnable = VK_FALSE;
    multisample.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlend{};
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.logicOpEnable = VK_FALSE;
    colorBlend.logicOp = VK_LOGIC_OP_COPY;
    colorBlend.blendConstants[0] = 0.0f;
    colorBlend.blendConstants[1] = 0.0f;
    colorBlend.blendConstants[2] = 0.0f;
    colorBlend.blendConstants[3] = 0.0f;
    colorBlend.attachmentCount = 1;
    colorBlend.pAttachments = &colorBlendAttachment;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;

    const std::array<VkDynamicState, 2> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = (u32)dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();

    std::array<VkPipelineShaderStageCreateInfo, SHADER_STAGE_COUNT> shaderStages{};
    const VkShaderModule vertexModule = _createShaderModule(device, "obj/shaders/scene.vertex.spv");
    shaderStages[(u32)ShaderStage::VertexInstance].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[(u32)ShaderStage::VertexInstance].stage = (VkShaderStageFlagBits)ShaderType::VS;
    shaderStages[(u32)ShaderStage::VertexInstance].module = vertexModule;
    shaderStages[(u32)ShaderStage::VertexInstance].pName = "main";

    const VkShaderModule fragmentModule = _createShaderModule(device, "obj/shaders/scene.fragment.spv");
    shaderStages[(u32)ShaderStage::Fragment].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[(u32)ShaderStage::Fragment].stage = (VkShaderStageFlagBits)ShaderType::FS;
    shaderStages[(u32)ShaderStage::Fragment].module = fragmentModule;
    shaderStages[(u32)ShaderStage::Fragment].pName = "main";

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pVertexInputState = &vertexInput;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisample;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlend;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.stageCount = SHADER_STAGE_COUNT;
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(device.getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS)
        throw std::runtime_error("Failed to assemble graphics pipeline.");

    vkDestroyShaderModule(device.getDevice(), vertexModule, nullptr);
    vkDestroyShaderModule(device.getDevice(), fragmentModule, nullptr);

    LDEBUG("Scene pipeline assembled: " << m_pipeline);
}

void ScenePipeline::destroy(const Device& device) {
    vkDestroyPipeline(device.getDevice(), m_pipeline, nullptr);
    for (u32 i = 0; i < VERTEX_BUFFER_COUNT; ++i)
        m_vertexBuffers[i].destroy(device);

    LDEBUG("Scene pipeline destroyed.");
}

/* ========================================================================== */

void ScenePipeline::record(
    const VkPipelineLayout layout,
    const DescriptorTable& descriptorTable,
    const ICommandBuffer* cmdBuffer
) {
    const std::array<VkDescriptorSet, DESCRIPTOR_SET_COUNT> descriptorSets = {
        descriptorTable[(DescriptorSetIndex)SetIndex::PerFrameData]->getSet(),
        descriptorTable[(DescriptorSetIndex)SetIndex::Textures]->getSet() };
    const std::array<VkDeviceSize, 1> offsets = { 0 };
    const std::array<VkBuffer, 1> vertexBuffers = { _getCurrentBuffer().getBuffer().getBuffer() };

    vkCmdBindDescriptorSets(
        cmdBuffer->getBuffer(),
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        layout,
        (u32)SceneDescriptorSet::First,
        DESCRIPTOR_SET_COUNT, descriptorSets.data(),
        0, nullptr);

    vkCmdBindPipeline(
        cmdBuffer->getBuffer(),
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_pipeline);

    vkCmdBindVertexBuffers(
        cmdBuffer->getBuffer(),
        0, vertexBuffers.size(),
        vertexBuffers.data(),
        offsets.data());

    vkCmdDraw(cmdBuffer->getBuffer(), 4, _getCurrentBuffer().getInstancesCount(), 0, 0);
}

#if ENABLE_FRUSTUM_CULLING

void ScenePipeline::initVertexBuffer(
    const Device& device,
    const game::GameState& gameState
) {
    for (u32 i = 0; i < VERTEX_BUFFER_COUNT; ++i)
        m_vertexBuffers[i].init(device, gameState);
}

void ScenePipeline::updateVertexBuffer(
    const Device& device,
    const game::GameState& gameState
) {
    _getCurrentBuffer().update(device, gameState);
    _switchBuffer();
}

#else

void ScenePipeline::initVertexBuffer(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const game::GameState& gameState
) {
    for (u32 i = 0; i < VERTEX_BUFFER_COUNT; ++i)
        m_vertexBuffers[i].init(device, cmdBuffer, gameState);
}

#endif

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

VertexBuffer& ScenePipeline::_getCurrentBuffer() {
    return m_vertexBuffers[m_currentBuffer];
}

void ScenePipeline::_switchBuffer() {
    m_currentBuffer = (m_currentBuffer + 1) % VERTEX_BUFFER_COUNT;
}

} // namespace vox::gfx
