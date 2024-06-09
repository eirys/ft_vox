/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_pipeline.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:06:29 by etran             #+#    #+#             */
/*   Updated: 2024/06/06 12:49:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shadow_pipeline.h"
#include "device.h"
#include "descriptor_table.h"
#include "icommand_buffer.h"
#include "vertex_buffer.h"

#include "debug.h"

namespace vox::gfx {

enum class ShadowDescriptorSet: u32 {
    Pfd = 0,

    First = Pfd,
    Last = Pfd
};

enum class SetIndex: u32 {
    PerFrameData    = (u32)DescriptorSetIndex::Pfd
};

static constexpr u32 DESCRIPTOR_SET_COUNT = enumSize<ShadowDescriptorSet>();

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void ShadowPipeline::init(
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
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;

    VkPipelineMultisampleStateCreateInfo multisample{};
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.sampleShadingEnable = VK_FALSE;
    multisample.minSampleShading = 1.0f;
    multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlend{};
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.logicOpEnable = VK_FALSE;
    colorBlend.logicOp = VK_LOGIC_OP_COPY;
    colorBlend.blendConstants[0] = 0.0f;
    colorBlend.blendConstants[1] = 0.0f;
    colorBlend.blendConstants[2] = 0.0f;
    colorBlend.blendConstants[3] = 0.0f;
    colorBlend.attachmentCount = 0;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencil.minDepthBounds = 0.0f;
	depthStencil.maxDepthBounds = 1.0f;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    const std::array<VkDynamicState, 2> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = (u32)dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();

    std::array<VkPipelineShaderStageCreateInfo, SHADER_STAGE_COUNT> shaderStages{};
    const VkShaderModule vertexModule = _createShaderModule(device, "obj/shaders/shadowmap.vertex.spv");
    shaderStages[(u32)ShaderStage::Vertex].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[(u32)ShaderStage::Vertex].stage = (VkShaderStageFlagBits)ShaderType::VS;
    shaderStages[(u32)ShaderStage::Vertex].module = vertexModule;
    shaderStages[(u32)ShaderStage::Vertex].pName = "main";

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

    LDEBUG("Shadow pipeline assembled: " << m_pipeline);
}

void ShadowPipeline::destroy(const Device& device) {
    vkDestroyPipeline(device.getDevice(), m_pipeline, nullptr);

    LDEBUG("Shadow pipeline destroyed.");
}

void ShadowPipeline::record(
    const VkPipelineLayout layout,
    const DescriptorTable& descriptorTable,
    const ICommandBuffer* cmdBuffer
) {
    const std::array<VkDescriptorSet, DESCRIPTOR_SET_COUNT> descriptorSets = {
        descriptorTable[(u32)SetIndex::PerFrameData]->getSet() };
    const std::array<VkDeviceSize, 1> offsets = { 0 };
    const std::array<VkBuffer, 1> vertexBuffers = { VertexBuffer::getBuffer().getBuffer() };

    vkCmdBindDescriptorSets(
        cmdBuffer->getBuffer(),
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        layout,
        (u32)ShadowDescriptorSet::First,
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

    vkCmdDraw(cmdBuffer->getBuffer(), 4, VertexBuffer::getInstancesCount(), 0, 0);
}

} // namespace vox::gfx