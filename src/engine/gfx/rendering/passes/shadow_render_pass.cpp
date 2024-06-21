/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_render_pass.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 12:53:21 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 02:36:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shadow_render_pass.h"
#include "device.h"
#include "icommand_buffer.h"
#include "render_decl.h"

#include <array>

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void ShadowRenderPass::init(const Device& device, const RenderPassInfo* info) {
    _createRenderPass(device, info);
    _importResources(device, info);
    _createTarget(device, info);

    LDEBUG("Shadow render pass initialized:" << m_vkRenderPass);
}

void ShadowRenderPass::destroy(const Device& device) {
    _destroyTarget(device);
    vkDestroyRenderPass(device.getDevice(), m_vkRenderPass, nullptr);
}

/* ========================================================================== */

void ShadowRenderPass::updateResources(const Device& device, const RenderPassInfo* info) {
    _destroyTarget(device);
    _createTarget(device, info);
}

void ShadowRenderPass::begin(const ICommandBuffer* cmdBuffer, const RecordInfo& recordInfo) const {
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

    std::array<VkClearValue, RESOURCE_COUNT> clearValues{};
    clearValues[(u32)Attachment::Depth].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.renderPass = m_vkRenderPass;
    beginInfo.framebuffer = m_targets[recordInfo.m_targetIndex];
    beginInfo.renderArea.offset = { 0, 0 };
    beginInfo.renderArea.extent = { m_width, m_height };
    beginInfo.clearValueCount = (u32)clearValues.size();
    beginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(cmdBuffer->getBuffer(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void ShadowRenderPass::_createRenderPass(const Device& device, const RenderPassInfo* info) {
    // Attachments
    std::array<VkAttachmentDescription, ATTACHMENT_COUNT> attachments{};

    attachments[(u32)Attachment::Depth].format = info->m_formats[(u32)Resource::DepthImage];
    attachments[(u32)Attachment::Depth].samples = info->m_samples[(u32)Resource::DepthImage];
    attachments[(u32)Attachment::Depth].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[(u32)Attachment::Depth].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[(u32)Attachment::Depth].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[(u32)Attachment::Depth].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[(u32)Attachment::Depth].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[(u32)Attachment::Depth].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    // Subpasses
    std::array<VkAttachmentReference, ATTACHMENT_COUNT> attachmentRefs{};

    attachmentRefs[(u32)Attachment::Depth].attachment = (u32)Attachment::Depth;
    attachmentRefs[(u32)Attachment::Depth].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 0;
    subpass.pDepthStencilAttachment = &attachmentRefs[(u32)Attachment::Depth];

    // Subpass dependency
	std::array<VkSubpassDependency, 2>	dependencies{};
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    // Render pass creation
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = (u32)attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = (u32)dependencies.size();
    renderPassInfo.pDependencies = dependencies.data();

    if (vkCreateRenderPass(device.getDevice(), &renderPassInfo, nullptr, &m_vkRenderPass) != VK_SUCCESS)
        throw std::runtime_error("failed to create shadow render pass");
    LDEBUG("Shadow render pass created.");
}

void ShadowRenderPass::_importResources(const Device& device, const RenderPassInfo* info) {
    m_width = info->m_renderPassWidth;
    m_height = info->m_renderPassHeight;

    const ShadowRenderPassInfo* shadowInfo = dynamic_cast<const ShadowRenderPassInfo*>(info);

    m_resources.resize(RESOURCE_COUNT, shadowInfo->m_texture);

    LDEBUG("Shadow render pass resources imported.");
}

void ShadowRenderPass::_createTarget(const Device& device, const RenderPassInfo* info) {
    m_targets.resize(info->m_targetCount);

    std::array<VkImageView, ATTACHMENT_COUNT> attachments = { m_resources[(u32)Attachment::Depth]->getView() };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = m_vkRenderPass;
    framebufferInfo.attachmentCount = ATTACHMENT_COUNT;
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = info->m_targetWidth;
    framebufferInfo.height = info->m_targetHeight;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device.getDevice(), &framebufferInfo, nullptr, m_targets.data()) != VK_SUCCESS)
        throw std::runtime_error("failed to create shadow frame buffer");
}

void ShadowRenderPass::_destroyTarget(const Device& device) {
    for (VkFramebuffer& target : m_targets) vkDestroyFramebuffer(device.getDevice(), target, nullptr);
}

} // namespace vox::gfx