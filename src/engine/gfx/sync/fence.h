/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fence.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 11:11:32 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 12:25:37 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>
#include "types.h"

namespace vox::gfx {

class Device;

/**
 * @brief Manager for VkFences. Synchronizes CPU and GPU.
*/
class Fence final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Fence() = default;
    ~Fence() = default;

    Fence(Fence&& other) = delete;
    Fence(const Fence& other) = delete;
    Fence& operator=(Fence&& other) = delete;
    Fence& operator=(const Fence& other) = delete;

    /* ====================================================================== */

    void init(const Device& device);
    void destroy(const Device& device);

    /* ====================================================================== */

    void await(const Device& device, const u64 timeout = UINT64_MAX) const;
    void reset(const Device& device) const;

    /* ====================================================================== */

    VkFence getFence() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkFence m_fence = VK_NULL_HANDLE;

}; // class Fence

} // namespace vox::gfx
