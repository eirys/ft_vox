/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_chain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 00:33:46 by etran             #+#    #+#             */
/*   Updated: 2024/03/20 15:14:01 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "swap_chain.h"
#include "core.h"
#include "device.h"
#include "window.h"
#include "gfx_semaphore.h"

#include <limits>
#include <stdexcept>
#include <algorithm>

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                               STATIC MEMBERS                               */
/* ========================================================================== */

VkFormat    SwapChain::ms_depthFormat = VK_FORMAT_UNDEFINED;

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

void SwapChain::init(const Core& core, const Device& device, const ui::Window& window) {
    if (ms_depthFormat == VK_FORMAT_UNDEFINED)
        throw std::runtime_error("Failed to find depth format");

    _createSwapChain(core, device, window);
    _createImages(device);

    LDEBUG("Swap chain initialized.");
}

void SwapChain::destroy(const Device& device) {
    for (u32 i = 0; i < m_imageViews.size(); ++i)
        vkDestroyImageView(device.getDevice(), m_imageViews[i], nullptr);
    vkDestroySwapchainKHR(device.getDevice(), m_swapChain, nullptr);

    LDEBUG("Swap chain destroyed.");
}

/* ========================================================================== */

bool SwapChain::acquireNextImage(const Device& device, const GfxSemaphore& semaphore) {
    VkResult result = vkAcquireNextImageKHR(device.getDevice(), m_swapChain, UINT64_MAX, semaphore.getSemaphore(), VK_NULL_HANDLE, &m_imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
        return false; // Require swap chain recreation
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        throw std::runtime_error("Failed to acquire swap chain image.");
    return true;
}

bool SwapChain::submitImage(const Device& device, const GfxSemaphore& semaphore) const {
    std::array<VkSemaphore, 1> waitSemaphores = { semaphore.getSemaphore() };

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = (u32)waitSemaphores.size();
    presentInfo.pWaitSemaphores = waitSemaphores.data();
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapChain;
    presentInfo.pImageIndices = &m_imageIndex;
    presentInfo.pResults = nullptr;

    VkResult result = vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        return false; // Require swap chain recreation
    else if (result != VK_SUCCESS)
        throw std::runtime_error("Failed to present swap chain image.");

    return true;
}

/* ========================================================================== */

VkFormat SwapChain::getImageFormat() const noexcept {
    return m_imageFormat;
}

const std::vector<VkImageView>& SwapChain::getImageViews() const noexcept {
    return m_imageViews;
}

u32 SwapChain::getImageIndex() const noexcept {
    return m_imageIndex;
}

VkExtent2D SwapChain::getImageExtent() const noexcept {
    return m_imageExtent;
}

/* ========================================================================== */

void SwapChain::setDepthFormat(const VkFormat format) noexcept {
    ms_depthFormat = format;
}

VkFormat SwapChain::getDepthFormat() noexcept {
    return ms_depthFormat;
}

/* ========================================================================== */
/*                                   PRIVATE                                  */
/* ========================================================================== */

static
VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const VkSurfaceFormatKHR& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    }
    return availableFormats[0];
}

static
VkPresentModeKHR _chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const VkPresentModeKHR& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return availablePresentMode;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

static
VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const ui::Window& window) {
    if (capabilities.currentExtent.width != std::numeric_limits<u32>::max())
        return capabilities.currentExtent;
    else {
        int width, height;
        window.retrieveFramebufferSize(width, height);

        VkExtent2D actualExtent = { (u32)width, (u32)height };
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }
}


void SwapChain::_createSwapChain(const Core& core, const Device& device, const ui::Window& window) {
    VkSwapchainKHR oldSwapChain = m_swapChain;

    SwapChainSupportDetails swapChainSupport = device.querySwapChainSupport(core);
    VkSurfaceFormatKHR  surfaceFormat = _chooseSwapSurfaceFormat(swapChainSupport.m_formats);
    VkPresentModeKHR    presentMode = _chooseSwapPresentMode(swapChainSupport.m_presentModes);
    VkExtent2D          extent = _chooseSwapExtent(swapChainSupport.m_capabilities, window);

    u32 imageCount = swapChainSupport.m_capabilities.minImageCount + 1;
    if (swapChainSupport.m_capabilities.maxImageCount > 0 && imageCount > swapChainSupport.m_capabilities.maxImageCount)
        imageCount = swapChainSupport.m_capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR swapInfo{};
    swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapInfo.surface = core.getSurface();
    swapInfo.minImageCount = imageCount;
    swapInfo.imageFormat = surfaceFormat.format;
    swapInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapInfo.imageExtent = extent;
    swapInfo.imageArrayLayers = 1;
    swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = device.getQueueFamilyIndices();
    std::array<u32, 2> queueFamilyIndices = { indices.m_graphicsFamily.value(), indices.m_presentFamily.value() };

    if (indices.m_graphicsFamily != indices.m_presentFamily) {
        // No image ownership transfer -> concurrent queue family sharing mode
        swapInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapInfo.queueFamilyIndexCount = (u32)queueFamilyIndices.size();
        swapInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    } else {
        swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapInfo.queueFamilyIndexCount = 0;
        swapInfo.pQueueFamilyIndices = nullptr;
    }

    swapInfo.preTransform = swapChainSupport.m_capabilities.currentTransform;
    swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapInfo.presentMode = presentMode;
    swapInfo.clipped = VK_TRUE;
    swapInfo.oldSwapchain = oldSwapChain;

    if (vkCreateSwapchainKHR(device.getDevice(), &swapInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        throw std::runtime_error("Failed to create swap chain.");

    if (oldSwapChain != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(device.getDevice(), oldSwapChain, nullptr);


    m_imageFormat = surfaceFormat.format;
    m_imageExtent = extent;
}

/* ========================================================================== */

static
VkImageView _createImageView(const Device& device, const VkImage vkImage, const VkFormat format) {
    VkImageView vkImageView;

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = vkImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device.getDevice(), &viewInfo, nullptr, &vkImageView) != VK_SUCCESS)
        throw std::runtime_error("Failed to create image view.");

    return vkImageView;
}

void SwapChain::_createImages(const Device& device) {
    u32 imageCount;
    vkGetSwapchainImagesKHR(device.getDevice(), m_swapChain, &imageCount, nullptr);
    m_images.resize(imageCount);
    vkGetSwapchainImagesKHR(device.getDevice(), m_swapChain, &imageCount, m_images.data());

    m_imageViews.reserve(imageCount);
    for (u32 imgIndex = 0; imgIndex < imageCount; ++imgIndex) {
        m_imageViews.emplace_back(_createImageView(device, m_images[imgIndex], m_imageFormat));
    }
}

} // namespace vox::gfx
