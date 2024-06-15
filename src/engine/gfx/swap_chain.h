/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_chain.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:16:45 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 21:54:28 by etran            ###   ########.fr       */
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

    static constexpr std::array<VkFormat, 5>    DEPTH_FORMAT_CANDIDATES = {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM };
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

    /* ====================================================================== */

    VkFormat                        getImageFormat() const noexcept;
    const std::vector<VkImageView>& getImageViews() const noexcept;
    u32                             getImageIndex() const noexcept;
    static VkExtent2D               getImageExtent() noexcept;

    static void     setDepthFormat(const VkFormat format) noexcept;
    static VkFormat getDepthFormat() noexcept;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static VkFormat             ms_depthFormat;
    static VkExtent2D           ms_imageExtent;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::vector<VkImage>        m_images;
    std::vector<VkImageView>    m_imageViews;
    VkSwapchainKHR              m_swapChain = VK_NULL_HANDLE;

    VkFormat                    m_imageFormat = VK_FORMAT_UNDEFINED;
    u32                         m_imageIndex;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void    _createSwapChain(const Core& core, const Device& device, const ui::Window& window);
    void    _createImages(const Device& device);

}; // class SwapChain

} // namespace vox::gfx
