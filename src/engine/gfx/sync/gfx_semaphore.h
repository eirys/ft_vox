/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_semaphore.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 10:43:33 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 12:31:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vulkan/vulkan.h>

namespace vox::gfx {

class Device;

/**
 * @brief Manager for VkSemaphores. Synchronizes GPU commands.
*/
class GfxSemaphore final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    GfxSemaphore() = default;
    ~GfxSemaphore() = default;

    GfxSemaphore(GfxSemaphore&& other) = delete;
    GfxSemaphore(const GfxSemaphore& other) = delete;
    GfxSemaphore& operator=(GfxSemaphore&& other) = delete;
    GfxSemaphore& operator=(const GfxSemaphore& other) = delete;

    /* ====================================================================== */

    void init(const Device& device);
    void destroy(const Device& device);

    /* ====================================================================== */

    void signal(const VkQueue queue);

    /* ====================================================================== */

    VkSemaphore getSemaphore() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    VkSemaphore m_semaphore;

}; // class GfxSemaphore

} // namespace vox::gfx