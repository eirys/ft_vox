/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_pipeline.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:48:36 by etran             #+#    #+#             */
/*   Updated: 2024/03/20 19:21:35 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "skybox_pipeline.h"
#include "device.h"
#include "render_pass.h"
#include "icommand_buffer.h"
#include "descriptor_table.h"

#include "debug.h"

#include <array>

namespace vox::gfx {

enum class SkyboxDescriptorSet: u32 {
    Mvp = 0,
    WorldData,

    First = Mvp,
    Last = WorldData
};

static constexpr u32 DESCRIPTOR_SET_COUNT = enumSize<SkyboxDescriptorSet>();

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void SkyboxPipeline::init(
    const Device& device,
    const RenderPassInfo* info,
    const VkPipelineLayout& pipelineLayout
) {
    const SkyboxRenderPassInfo* renderPassInfo = dynamic_cast<const SkyboxRenderPassInfo*>(info);
    m_renderPass = renderPassInfo->m_scenePass;

    VkPipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

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
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;
    rasterizer.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisample{};
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.minSampleShading = 1.0f;
    multisample.rasterizationSamples = device.getMsaaCount();

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlend{};
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.attachmentCount = 1;
    colorBlend.pAttachments = &colorBlendAttachment;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_FALSE;
    depthStencil.depthWriteEnable = VK_FALSE;
    depthStencil.back.compareOp = VK_COMPARE_OP_ALWAYS;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;

    const std::array<VkDynamicState, 2> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = (u32)dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();

    std::array<VkPipelineShaderStageCreateInfo, SHADER_STAGE_COUNT> shaderStages{};
    const VkShaderModule vertexModule = _createShaderModule(device, "obj/shaders/scene.vertex.spv");
    shaderStages[(u32)ShaderStage::Vertex].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[(u32)ShaderStage::Vertex].stage = (VkShaderStageFlagBits)ShaderType::VS;
    shaderStages[(u32)ShaderStage::Vertex].module = vertexModule;
    shaderStages[(u32)ShaderStage::Vertex].pName = "main";

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
    pipelineInfo.renderPass = m_renderPass->getRenderPass();
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


    LDEBUG("Skybox pipeline initialized.");
}

void SkyboxPipeline::destroy(const Device& device) {
    vkDestroyPipeline(device.getDevice(), m_pipeline, nullptr);

    LDEBUG("Scene pipeline destroyed.");
}

/* ========================================================================== */

void SkyboxPipeline::record(
    const VkPipelineLayout layout,
    const DescriptorTable& descriptorTable,
    const ICommandBuffer* cmdBuffer,
    const RecordInfo& drawInfo
) {
    m_renderPass->begin(cmdBuffer, drawInfo);

    VkViewport viewport{};
    viewport.width = (f32)m_renderPass->getWidth();
    viewport.height = (f32)m_renderPass->getHeight();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmdBuffer->getBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = { m_renderPass->getWidth(), m_renderPass->getHeight() };
    vkCmdSetScissor(cmdBuffer->getBuffer(), 0, 1, &scissor);

    std::array<VkDescriptorSet, DESCRIPTOR_SET_COUNT> descriptorSets = {
        descriptorTable[(u32)SkyboxDescriptorSet::Mvp]->getSet(),
        descriptorTable[(u32)SkyboxDescriptorSet::WorldData]->getSet()
    };

    vkCmdBindDescriptorSets(
        cmdBuffer->getBuffer(),
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        layout,
        (u32)SkyboxDescriptorSet::First,
        DESCRIPTOR_SET_COUNT, descriptorSets.data(),
        0, nullptr);

    vkCmdBindPipeline(
        cmdBuffer->getBuffer(),
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_pipeline);

    // Draw skybox
    vkCmdDraw(cmdBuffer->getBuffer(), 4, 1, 0, 0);
    LDEBUG("Drawing Skybox");

    m_renderPass->end(cmdBuffer);
}

RenderPass* SkyboxPipeline::getRenderPass() const noexcept {
    return m_renderPass;
}

} // namespace vox::gfx
