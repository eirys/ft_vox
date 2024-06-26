/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deferred_pipeline.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:09:22 by etran             #+#    #+#             */
/*   Updated: 2024/06/25 15:05:36 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "deferred_pipeline.h"
#include "device.h"
#include "icommand_buffer.h"
#include "vertex_buffer.h"
#include "pipeline_layout.h"
#include "vox_decl.h"
#include "debug.h"

#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

__attribute__ ((optnone))
void DeferredPipeline::init(
    const Device& device,
    const VkRenderPass& renderPass,
    const PipelineLayout& pipelineLayout
) {
    m_pipelineLayout = &pipelineLayout;

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
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;

    VkPipelineMultisampleStateCreateInfo multisample{};
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.sampleShadingEnable = VK_FALSE;
    multisample.minSampleShading = 1.0f;
    multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

#if ENABLE_SSAO
    constexpr u32 COLOR_ATT = 5;
#else
    constexpr u32 COLOR_ATT = 3;
#endif

    std::array<VkPipelineColorBlendAttachmentState, COLOR_ATT> colorBlendAttachments{};
    for (auto& attachment : colorBlendAttachments) {
        attachment.colorWriteMask = 0x0f;
        attachment.blendEnable = VK_FALSE;
    }

    VkPipelineColorBlendStateCreateInfo colorBlend{};
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.logicOpEnable = VK_FALSE;
    colorBlend.logicOp = VK_LOGIC_OP_COPY;
    colorBlend.attachmentCount = colorBlendAttachments.size();
    colorBlend.pAttachments = colorBlendAttachments.data();

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
    const VkShaderModule vertexModule = _createShaderModule(device, "obj/shaders/deferred.vertex.spv");
    shaderStages[(u32)ShaderStage::Vertex].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[(u32)ShaderStage::Vertex].stage = (VkShaderStageFlagBits)ShaderType::VS;
    shaderStages[(u32)ShaderStage::Vertex].module = vertexModule;
    shaderStages[(u32)ShaderStage::Vertex].pName = "main";

    const VkShaderModule fragmentModule = _createShaderModule(device, "obj/shaders/deferred.fragment.spv");
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
    pipelineInfo.layout = m_pipelineLayout->getLayout();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(device.getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS)
        throw std::runtime_error("Failed to assemble graphics pipeline.");

    vkDestroyShaderModule(device.getDevice(), vertexModule, nullptr);
    vkDestroyShaderModule(device.getDevice(), fragmentModule, nullptr);

    LDEBUG("Deferred pipeline assembled: " << m_pipeline);
}

void DeferredPipeline::destroy(const Device& device) {
    vkDestroyPipeline(device.getDevice(), m_pipeline, nullptr);

    LDEBUG("Deferred pipeline destroyed.");
}

/* ========================================================================== */

void DeferredPipeline::record(const ICommandBuffer* cmdBuffer) const {
    cmdBuffer->bindDescriptorSets(*m_pipelineLayout);
    cmdBuffer->bindPipeline(m_pipeline);
    VertexBuffer::bind(cmdBuffer);

    vkCmdDraw(cmdBuffer->getBuffer(), 4, VertexBuffer::getInstancesCount(), 0, 0);
}

} // namespace vox::gfx