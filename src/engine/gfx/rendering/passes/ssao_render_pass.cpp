/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssao_render_pass.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 18:01:25 by etran             #+#    #+#             */
/*   Updated: 2024/06/19 12:16:49 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ssao_render_pass.h"
#include "device.h"
#include "icommand_buffer.h"
#include "render_decl.h"

#include <array>
#include <stdexcept>

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void SSAORenderPass::init(const Device& device, const RenderPassInfo* info) {
    _createRenderPass(device, info);
    _importResources(device, info);
    _createTargets(device, info);

    LDEBUG("SSAO render pass initialized:" << m_vkRenderPass);
}

void SSAORenderPass::destroy(const Device& device) {
    _destroyTargets(device);
    vkDestroyRenderPass(device.getDevice(), m_vkRenderPass, nullptr);
}

void SSAORenderPass::updateResources(const Device& device, const RenderPassInfo* info) {
}

void SSAORenderPass::begin(const ICommandBuffer* cmdBuffer, const RecordInfo& recordInfo) const {
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.renderPass = m_vkRenderPass;
    beginInfo.framebuffer = m_targets[recordInfo.m_targetIndex];
    beginInfo.renderArea.extent = { m_width, m_height };
    beginInfo.clearValueCount = (u32)clearValues.size();
    beginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(cmdBuffer->getBuffer(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.width = (f32)m_width;
    viewport.height = (f32)m_height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmdBuffer->getBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = { m_width, m_height };
    vkCmdSetScissor(cmdBuffer->getBuffer(), 0, 1, &scissor);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void SSAORenderPass::_createRenderPass(const Device& device, const RenderPassInfo* info) {
    // Attachments
    std::array<VkAttachmentDescription, ATTACHMENT_COUNT> attachments{};

    attachments[(u32)Attachment::Color].format = info->m_formats[(u32)Resource::Color];
    attachments[(u32)Attachment::Color].samples = info->m_samples[(u32)Resource::Color];
    attachments[(u32)Attachment::Color].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[(u32)Attachment::Color].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[(u32)Attachment::Color].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[(u32)Attachment::Color].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[(u32)Attachment::Color].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[(u32)Attachment::Color].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    // Subpasses
    std::array<VkAttachmentReference, ATTACHMENT_COUNT> attachmentRefs{};
    attachmentRefs[(u32)Attachment::Color].attachment = (u32)Attachment::Color;
    attachmentRefs[(u32)Attachment::Color].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = attachmentRefs.size();
    subpass.pColorAttachments = attachmentRefs.data();

    // Subpass dependencies
    std::array<VkSubpassDependency, 2> dependencies{};
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    // Render pass
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = dependencies.size();
    renderPassInfo.pDependencies = dependencies.data();

    if (vkCreateRenderPass(device.getDevice(), &renderPassInfo, nullptr, &m_vkRenderPass) != VK_SUCCESS)
        throw std::runtime_error("failed to create deferred render pass");

    LDEBUG("SSAO render pass created.");
}

void SSAORenderPass::_importResources(const Device& device, const RenderPassInfo* info) {
    const SSAORenderPassInfo* ssaoInfo = dynamic_cast<const SSAORenderPassInfo*>(info);

    m_width = ssaoInfo->m_renderPassWidth;
    m_height = ssaoInfo->m_renderPassHeight;

    m_resources = { &ssaoInfo->m_texture };

    LDEBUG("SSAO render pass resources created.");
}

void SSAORenderPass::_createTargets(const Device& device, const RenderPassInfo* info) {
    m_targets.resize(info->m_targetCount);

    std::array<VkImageView, ATTACHMENT_COUNT> attachments = {
        m_resources[(u32)Attachment::Color]->getView() };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = m_vkRenderPass;
    framebufferInfo.attachmentCount = ATTACHMENT_COUNT;
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = info->m_targetWidth;
    framebufferInfo.height = info->m_targetHeight;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device.getDevice(), &framebufferInfo, nullptr, m_targets.data()) != VK_SUCCESS)
        throw std::runtime_error("failed to create deferred frame buffer");
}

void SSAORenderPass::_destroyTargets(const Device& device) {
    for (VkFramebuffer& target: m_targets) vkDestroyFramebuffer(device.getDevice(), target, nullptr);
}

} // namespace vox::gfx