/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_chain.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:16:45 by etran             #+#    #+#             */
/*   Updated: 2024/03/05 09:49:56 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vulkan/vulkan.h>
# include <array>
# include <vector>
# include "types.h"

namespace ui {

class Window;

}

namespace vox::gfx {

class Core;
class Device;
class GfxSemaphore;

class SwapChain final {
public:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr std::array<VkFormat, 3>    DEPTH_FORMAT_CANDIDATES = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT };
    static constexpr VkImageTiling              DEPTH_TILING = VK_IMAGE_TILING_OPTIMAL;
    static constexpr VkFormatFeatureFlagBits    DEPTH_FEATURES = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    SwapChain() = default;
    ~SwapChain() = default;

    SwapChain(SwapChain&& other) = delete;
    SwapChain(const SwapChain& other) = delete;
    SwapChain& operator=(SwapChain&& other) = delete;
    SwapChain& operator=(const SwapChain& other) = delete;

    /* ====================================================================== */

    void    init(const Core& core, const Device& device, const ui::Window& window);
    void    destroy(const Device& device);

    bool    acquireNextImage(const Device& device, const GfxSemaphore& semaphore);
    bool    submitImage(const Device& device, const GfxSemaphore& semaphore) const;

    static
    void    setDepthFormat(const VkFormat format) noexcept;

    /* ====================================================================== */

    u32     getImageIndex() const noexcept;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static VkFormat             ms_depthFormat;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::vector<VkImage>        m_images;
    std::vector<VkImageView>    m_imageViews;
    VkSwapchainKHR              m_swapChain = VK_NULL_HANDLE;

    VkFormat                    m_imageFormat = VK_FORMAT_UNDEFINED;
    u32                         m_imageIndex;
    VkExtent2D                  m_imageExtent = { 0, 0 };

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createSwapChain(const Core& core, const Device& device, const ui::Window& window);
    void    _createImages(const Device& device);

}; // class SwapChain

} // namespace vox::gfx
