/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deferred_render_pass.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:50:52 by etran             #+#    #+#             */
/*   Updated: 2024/06/21 14:36:33 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "deferred_render_pass.h"
#include "device.h"
#include "debug.h"
#include "icommand_buffer.h"
#include "render_decl.h"

#include <array>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void DeferredRenderPass::init(const Device& device, const RenderPassInfo* info) {
    _createRenderPass(device, info);
    _importResources(device, info);
    _createTargets(device, info);

    LDEBUG("Deferred render pass initialized:" << m_vkRenderPass);
}

void DeferredRenderPass::destroy(const Device& device) {
    _destroyTargets(device);

    // Delete local resource
    m_resources[(u32)Resource::DepthImage]->destroy(device);
    delete m_resources[(u32)Resource::DepthImage];

    vkDestroyRenderPass(device.getDevice(), m_vkRenderPass, nullptr);
}

void DeferredRenderPass::updateResources(const Device& device, const RenderPassInfo* info) {
}

/* ========================================================================== */

void DeferredRenderPass::begin(const ICommandBuffer* cmdBuffer, const RecordInfo& recordInfo) const {
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
    clearValues[(u32)Attachment::ColorPosition].color = {{ 0.0f, 0.0f, 0.0f, 0.0f }};
    clearValues[(u32)Attachment::ColorNormal].color = {{ 0.0f, 0.0f, 0.0f, 0.0f }};
    clearValues[(u32)Attachment::ColorAlbedo].color = {{ 0.0f, 0.0f, 0.0f, 0.0f }};
    clearValues[(u32)Attachment::ColorViewNormal].color = {{ 0.0f, 0.0f, 0.0f, 0.0f }};
    clearValues[(u32)Attachment::Depth].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.renderPass = m_vkRenderPass;
    beginInfo.framebuffer = m_targets[recordInfo.m_targetIndex];
    beginInfo.renderArea.extent = { m_width, m_height };
    beginInfo.clearValueCount = (u32)clearValues.size();
    beginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(cmdBuffer->getBuffer(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void DeferredRenderPass::_createRenderPass(const Device& device, const RenderPassInfo* info) {
    const DeferredRenderPassInfo* deferredInfo = dynamic_cast<const DeferredRenderPassInfo*>(info);

    // Attachments
    std::array<VkAttachmentDescription, ATTACHMENT_COUNT> attachments{};

    auto createAtt = [](VkFormat format, VkSampleCountFlagBits sampleCount){
        VkAttachmentDescription attDesc{};
        attDesc.format = format;
        attDesc.samples = sampleCount;
        attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attDesc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        return attDesc;
    };

    attachments[(u32)Attachment::ColorPosition] = createAtt(deferredInfo->m_formats[(u32)Resource::ColorPosition], deferredInfo->m_samples[(u32)Resource::ColorPosition]);
    attachments[(u32)Attachment::ColorNormal] = createAtt(deferredInfo->m_formats[(u32)Resource::ColorNormal], deferredInfo->m_samples[(u32)Resource::ColorNormal]);
    attachments[(u32)Attachment::ColorAlbedo] = createAtt(deferredInfo->m_formats[(u32)Resource::ColorAlbedo], deferredInfo->m_samples[(u32)Resource::ColorAlbedo]);
    attachments[(u32)Attachment::ColorViewNormal] = createAtt(deferredInfo->m_formats[(u32)Resource::ColorViewNormal], deferredInfo->m_samples[(u32)Resource::ColorViewNormal]);

    attachments[(u32)Attachment::Depth].format = deferredInfo->m_formats[(u32)Resource::DepthImage];
    attachments[(u32)Attachment::Depth].samples = deferredInfo->m_samples[(u32)Resource::DepthImage];
    attachments[(u32)Attachment::Depth].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[(u32)Attachment::Depth].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[(u32)Attachment::Depth].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[(u32)Attachment::Depth].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[(u32)Attachment::Depth].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[(u32)Attachment::Depth].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    // Subpasses
    std::array<VkAttachmentReference, ATTACHMENT_COUNT> attachmentRefs{};

    attachmentRefs[(u32)Attachment::ColorPosition].attachment = (u32)Attachment::ColorPosition;
    attachmentRefs[(u32)Attachment::ColorPosition].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    attachmentRefs[(u32)Attachment::ColorNormal].attachment = (u32)Attachment::ColorNormal;
    attachmentRefs[(u32)Attachment::ColorNormal].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    attachmentRefs[(u32)Attachment::ColorViewNormal].attachment = (u32)Attachment::ColorViewNormal;
    attachmentRefs[(u32)Attachment::ColorViewNormal].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    attachmentRefs[(u32)Attachment::ColorAlbedo].attachment = (u32)Attachment::ColorAlbedo;
    attachmentRefs[(u32)Attachment::ColorAlbedo].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    attachmentRefs[(u32)Attachment::Depth].attachment = (u32)Attachment::Depth;
    attachmentRefs[(u32)Attachment::Depth].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 4;
    subpass.pColorAttachments = attachmentRefs.data();
    subpass.pDepthStencilAttachment = &attachmentRefs[(u32)Attachment::Depth];

    // Subpass dependencies
    std::array<VkSubpassDependency, 2> dependencies{};
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
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

    LDEBUG("Deferred render pass created.");
}

void DeferredRenderPass::_importResources(const Device& device, const RenderPassInfo* info) {
    const DeferredRenderPassInfo* deferredInfo = dynamic_cast<const DeferredRenderPassInfo*>(info);

    m_width = deferredInfo->m_renderPassWidth;
    m_height = deferredInfo->m_renderPassHeight;

    ImageMetaData textureData{};
    textureData.m_format = deferredInfo->m_formats[(u32)Resource::DepthImage];
    textureData.m_width = m_width;
    textureData.m_height = m_width;
    textureData.m_sampleCount = VK_SAMPLE_COUNT_1_BIT;
    textureData.m_usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                          VK_IMAGE_USAGE_SAMPLED_BIT;
    textureData.m_aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
    ImageBuffer* depthImg = new ImageBuffer(true);
    depthImg->initImage(device, std::move(textureData));
    depthImg->initView(device);

    m_resources = { &deferredInfo->m_positionTexture, &deferredInfo->m_normalTexture, &deferredInfo->m_albedoTexture, &deferredInfo->m_normalViewTexture, depthImg };

    LDEBUG("Deferred render pass resources created.");
}

void DeferredRenderPass::_createTargets(const Device& device, const RenderPassInfo* info) {
    m_targets.resize(info->m_targetCount);

    std::array<VkImageView, ATTACHMENT_COUNT> attachments = {
        m_resources[(u32)Attachment::ColorPosition]->getView(),
        m_resources[(u32)Attachment::ColorNormal]->getView(),
        m_resources[(u32)Attachment::ColorAlbedo]->getView(),
        m_resources[(u32)Attachment::ColorViewNormal]->getView(),
        m_resources[(u32)Attachment::Depth]->getView() };

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

void DeferredRenderPass::_destroyTargets(const Device& device) {
    for (VkFramebuffer& target: m_targets) vkDestroyFramebuffer(device.getDevice(), target, nullptr);
}

} // namespace vox::gfx