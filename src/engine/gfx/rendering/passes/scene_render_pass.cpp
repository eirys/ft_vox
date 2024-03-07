/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_render_pass.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:15:26 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 12:22:41 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene_render_pass.h"
#include "device.h"

#include <array>
#include <stdexcept>

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void SceneRenderPass::init(const Device& device, const RenderPassInfo& info) {
    _createRenderPass(device, info);
    _createResources(device, info);
    _createTarget(device, info);
}

void SceneRenderPass::destroy(const Device& device) {
    _destroyTarget(device);
    _destroyResources(device);
    vkDestroyRenderPass(device.getDevice(), m_vkRenderPass, nullptr);
}

void SceneRenderPass::updateResources(const Device& device, const RenderPassInfo& info) {
    _destroyTarget(device);
    _destroyResources(device);
    _createResources(device, info);
    _createTarget(device, info);
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

void SceneRenderPass::_createRenderPass(const Device& device, const RenderPassInfo& info) {
    // Attachments
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = info.m_formats[(u32)SceneResource::ColorImage];
    colorAttachment.samples = info.m_samples[(u32)SceneResource::ColorImage];
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = info.m_formats[(u32)SceneResource::DepthImage];
    depthAttachment.samples = info.m_samples[(u32)SceneResource::DepthImage];
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = info.m_formats[(u32)SceneResource::ColorImage];
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT; // For resolve, we only need 1 sample
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    std::array<VkAttachmentDescription, 3> attachments = {
        colorAttachment,
        depthAttachment,
        colorAttachmentResolve };

    // Subpasses
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = (u32)SceneResource::ColorImage;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = (u32)SceneResource::DepthImage;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = (u32)SceneResource::Last + 1;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.pResolveAttachments = &colorAttachmentResolveRef;

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
}

void SceneRenderPass::_createResources(const Device& device, const RenderPassInfo& info) {
    m_width = info.m_renderPassWidth;
    m_height = info.m_renderPassHeight;

    m_resources.resize(RESOURCE_COUNT);

    ImageMetaData colorImageMetaData{};
    colorImageMetaData.m_width = m_width;
    colorImageMetaData.m_height = m_height;
    colorImageMetaData.m_format = info.m_formats[(u32)SceneResource::ColorImage];
    colorImageMetaData.m_sampleCount = info.m_samples[(u32)SceneResource::ColorImage];
    colorImageMetaData.m_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
    colorImageMetaData.m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
    m_resources[(u32)SceneResource::ColorImage].init(device, std::move(colorImageMetaData));

    ImageMetaData depthImageMetaData{};
    depthImageMetaData.m_width = m_width;
    depthImageMetaData.m_height = m_height;
    depthImageMetaData.m_format = info.m_formats[(u32)SceneResource::DepthImage];
    depthImageMetaData.m_sampleCount = info.m_samples[(u32)SceneResource::DepthImage];
    depthImageMetaData.m_usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    depthImageMetaData.m_aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
    m_resources[(u32)SceneResource::DepthImage].init(device, std::move(depthImageMetaData));
}

void SceneRenderPass::_createTarget(const Device& device, const RenderPassInfo& info) {
    const SceneRenderPassInfo& scenePassInfo = dynamic_cast<const SceneRenderPassInfo&>(info);

    m_targets.resize(scenePassInfo.m_swapChainImageViews.size());

    std::array<VkImageView, 3> attachments = {
        m_resources[(u32)SceneResource::ColorImage].getView(),
        m_resources[(u32)SceneResource::DepthImage].getView() };

    for (u32 i = 0; i < m_targets.size(); ++i) {
        attachments[2] = scenePassInfo.m_swapChainImageViews[i];

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_vkRenderPass;
        framebufferInfo.attachmentCount = (u32)attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_width;
        framebufferInfo.height = m_height;
        framebufferInfo.layers = 1;

        VkFramebuffer* target = m_targets.data() + i;
        if (vkCreateFramebuffer(device.getDevice(), &framebufferInfo, nullptr, target) != VK_SUCCESS)
            throw std::runtime_error("failed to create scene frame buffer");
    }
}

void SceneRenderPass::_destroyTarget(const Device& device) {
    for (VkFramebuffer& target : m_targets) vkDestroyFramebuffer(device.getDevice(), target, nullptr);
}

void SceneRenderPass::_destroyResources(const Device& device) {
    for (ImageBuffer& resource: m_resources) resource.destroy(device);
}

} // namespace vox::gfx
