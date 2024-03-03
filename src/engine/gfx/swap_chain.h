/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_chain.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 21:16:45 by etran             #+#    #+#             */
/*   Updated: 2024/03/04 00:35:39 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vulkan/vulkan.h>
# include <array>
# include "types.h"

namespace vox::gfx {

class Device;
class Window;
class Semaphores;

class SwapChain final {
public:
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

    void    init(const Device& device, const Window& window);
    void    destroy(const Device& device);

    bool    acquireNextImage(const Device& device, const Semaphores& semaphores);
    bool    submitImage(const Device& device, const Semaphores& semaphores) const;

    /* ====================================================================== */

    static
    void    setDepthFormat(const VkFormat format) noexcept;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr std::array<VkFormat, 3>    DEPTH_FORMAT_CANDIDATES = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT };
    static constexpr VkImageTiling              DEPTH_TILING = VK_IMAGE_TILING_OPTIMAL;
    static constexpr VkFormatFeatureFlagBits    DEPTH_FEATURES = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    static VkFormat                             ms_depthFormat;

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    u32 m_imageIndex;

}; // class SwapChain

} // namespace vox::gfx
