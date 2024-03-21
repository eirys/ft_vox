/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_render_pass.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:15:26 by etran             #+#    #+#             */
/*   Updated: 2024/03/21 02:09:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_render_pass.h"
#include "device.h"
#include "render_decl.h"
#include "icommand_buffer.h"

#include <array>
#include <stdexcept>

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void MainRenderPass::init(const Device& device, const RenderPassInfo* info) {
    _createRenderPass(device, info);
    _createResources(device, info);
    _createTarget(device, info);

    LDEBUG("Scene render pass initialized");
}

void MainRenderPass::destroy(const Device& device) {
    _destroyTarget(device);
    _destroyResources(device);
    vkDestroyRenderPass(device.getDevice(), m_vkRenderPass, nullptr);
}

/* ========================================================================== */

void MainRenderPass::updateResources(const Device& device, const RenderPassInfo* info) {
    _destroyTarget(device);
    _destroyResources(device);
    _createResources(device, info);
    _createTarget(device, info);
}

void MainRenderPass::begin(const ICommandBuffer* cmdBuffer, const RecordInfo& recordInfo) {
    std::array<VkClearValue, RESOURCE_COUNT> clearValues{};
    clearValues[(u32)Attachment::Color].color = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
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

void MainRenderPass::_createRenderPass(const Device& device, const RenderPassInfo* info) {
    const MainRenderPassInfo* scenePassInfo = dynamic_cast<const MainRenderPassInfo*>(info);

    // Attachments
    std::array<VkAttachmentDescription, ATTACHMENT_COUNT> attachments{};

    attachments[(u32)Attachment::Color].format = scenePassInfo->m_formats[(u32)Resource::ColorImage];
    attachments[(u32)Attachment::Color].samples = scenePassInfo->m_samples[(u32)Resource::ColorImage];
    attachments[(u32)Attachment::Color].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[(u32)Attachment::Color].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[(u32)Attachment::Color].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[(u32)Attachment::Color].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[(u32)Attachment::Color].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[(u32)Attachment::Color].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    attachments[(u32)Attachment::Depth].format = scenePassInfo->m_formats[(u32)Resource::DepthImage];
    attachments[(u32)Attachment::Depth].samples = scenePassInfo->m_samples[(u32)Resource::DepthImage];
    attachments[(u32)Attachment::Depth].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[(u32)Attachment::Depth].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[(u32)Attachment::Depth].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[(u32)Attachment::Depth].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[(u32)Attachment::Depth].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[(u32)Attachment::Depth].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachments[(u32)Attachment::ColorResolve].format = scenePassInfo->m_formats[(u32)Resource::ColorImage];
    attachments[(u32)Attachment::ColorResolve].samples = VK_SAMPLE_COUNT_1_BIT; // For resolve, we only need 1 sample
    attachments[(u32)Attachment::ColorResolve].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[(u32)Attachment::ColorResolve].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[(u32)Attachment::ColorResolve].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[(u32)Attachment::ColorResolve].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[(u32)Attachment::ColorResolve].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[(u32)Attachment::ColorResolve].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Subpasses
    std::array<VkAttachmentReference, ATTACHMENT_COUNT> attachmentRefs{};

    attachmentRefs[(u32)Attachment::Color].attachment = (u32)Attachment::Color;
    attachmentRefs[(u32)Attachment::Color].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    attachmentRefs[(u32)Attachment::Depth].attachment = (u32)Attachment::Depth;
    attachmentRefs[(u32)Attachment::Depth].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachmentRefs[(u32)Attachment::ColorResolve].attachment = (u32)Attachment::ColorResolve;
    attachmentRefs[(u32)Attachment::ColorResolve].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &attachmentRefs[(u32)Attachment::Color];
    subpass.pDepthStencilAttachment = &attachmentRefs[(u32)Attachment::Depth];
    subpass.pResolveAttachments = &attachmentRefs[(u32)Attachment::ColorResolve];

    // Subpass dependency
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    // Render pass creation
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = (u32)attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(device.getDevice(), &renderPassInfo, nullptr, &m_vkRenderPass) != VK_SUCCESS)
        throw std::runtime_error("failed to create scene render pass");
    LDEBUG("Scene render pass created.");
}

void MainRenderPass::_createResources(const Device& device, const RenderPassInfo* info) {
    const MainRenderPassInfo* scenePassInfo = dynamic_cast<const MainRenderPassInfo*>(info);

    m_width = scenePassInfo->m_renderPassWidth;
    m_height = scenePassInfo->m_renderPassHeight;

    m_resources.resize(RESOURCE_COUNT);

    ImageMetaData colorImageMetaData{};
    colorImageMetaData.m_width = m_width;
    colorImageMetaData.m_height = m_height;
    colorImageMetaData.m_format = scenePassInfo->m_formats[(u32)Resource::ColorImage];
    colorImageMetaData.m_sampleCount = scenePassInfo->m_samples[(u32)Resource::ColorImage];
    colorImageMetaData.m_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
    colorImageMetaData.m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
    m_resources[(u32)Resource::ColorImage].initImage(device, std::move(colorImageMetaData));
    m_resources[(u32)Resource::ColorImage].initView(device);

    ImageMetaData depthImageMetaData{};
    depthImageMetaData.m_width = m_width;
    depthImageMetaData.m_height = m_height;
    depthImageMetaData.m_format = scenePassInfo->m_formats[(u32)Resource::DepthImage];
    depthImageMetaData.m_sampleCount = scenePassInfo->m_samples[(u32)Resource::DepthImage];
    depthImageMetaData.m_usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    depthImageMetaData.m_aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
    m_resources[(u32)Resource::DepthImage].initImage(device, std::move(depthImageMetaData));
    m_resources[(u32)Resource::DepthImage].initView(device);

    LDEBUG("Scene render pass resources created.");
}

void MainRenderPass::_createTarget(const Device& device, const RenderPassInfo* info) {
    const MainRenderPassInfo* scenePassInfo = dynamic_cast<const MainRenderPassInfo*>(info);

    m_targets.resize(scenePassInfo->m_targetCount);

    std::array<VkImageView, ATTACHMENT_COUNT> attachments = {
        m_resources[(u32)Attachment::Color].getView(),
        m_resources[(u32)Attachment::Depth].getView() };

    for (u32 i = 0; i < scenePassInfo->m_targetCount; ++i) {
        attachments[(u32)Attachment::ColorResolve] = scenePassInfo->m_swapChainImageViews[i];

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_vkRenderPass;
        framebufferInfo.attachmentCount = ATTACHMENT_COUNT;
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = scenePassInfo->m_targetWidth;
        framebufferInfo.height = scenePassInfo->m_targetHeight;
        framebufferInfo.layers = 1;

        VkFramebuffer* target = m_targets.data() + i;
        if (vkCreateFramebuffer(device.getDevice(), &framebufferInfo, nullptr, target) != VK_SUCCESS)
            throw std::runtime_error("failed to create scene frame buffer");
    }
}

void MainRenderPass::_destroyTarget(const Device& device) {
    for (VkFramebuffer& target : m_targets) vkDestroyFramebuffer(device.getDevice(), target, nullptr);
}

void MainRenderPass::_destroyResources(const Device& device) {
    for (u32 i = 0; i < RESOURCE_COUNT; ++i) m_resources[i].destroy(device);
}

} // namespace vox::gfx
